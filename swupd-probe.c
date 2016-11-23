/*
 * This program is part of the Clear Linux Project
 *
 * Copyright 2016 Intel Corporation
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *     Auke Kok <auke-jan.h.kok@intel.com>
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

#define TELEMETRY_DIR "/var/lib/swupd/telemetry"
#define TELEMETRY_CLASS "org.clearlinux/swupd-client/testing"

static uid_t uid;
static gid_t gid;

static bool get_creds_from_name(const char *name)
{
	struct passwd *p;

	if (!name) {
		return false;
	}

	p = getpwnam(name);
	if (!p) {
		return false;
	}

	uid = p->pw_uid;
	gid = p->pw_gid;

	return true;
}

/*
 * Find usable creds to deliver telemetry record.
 */
static void get_creds(void)
{
	if (get_creds_from_name("telemetry"))
		return;
	if (get_creds_from_name("nobody"))
		return;
	uid = 65534;
	gid = 65534;
}

static void deliver_payload(const char *filename)
{
	pid_t pid;
	int fd;
	char *tmp;
	char *version;
	char *severity;

	tmp = strdup(filename);
	if (!tmp) {
		perror("strdup");
		exit(EXIT_FAILURE);
	}

	version = strtok(tmp, ".");
	if (!version) {
		fprintf(stderr, "%s: does not look like a telemetry record\n", filename);
		exit(EXIT_FAILURE);
	}

	severity = strtok(NULL, ".");
	if (!severity) {
		fprintf(stderr, "%s: does not look like a telemetry record\n", filename);
		exit(EXIT_FAILURE);
	}

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror(filename);
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {
		/* child */
		if (dup2(fd, STDIN_FILENO) < 0) {
			perror("dup2");
			exit(EXIT_FAILURE);
		}

		close(fd);

		/* drop privileges */
		if (setgroups(0, NULL)) {
			perror("setgroups");
			exit(EXIT_FAILURE);
		}
		if (setgid(gid)) {
			perror("setgid");
			exit(EXIT_FAILURE);
		}
		if (setuid(uid)) {
			perror("setuid");
			exit(EXIT_FAILURE);
		}
		if (chdir("/")) {
			perror("chdir");
			exit(EXIT_FAILURE);
		}

		execl("/usr/bin/telem-record-gen", "/usr/bin/telem-record-gen",
			"--class", TELEMETRY_CLASS,
			"--severity", severity,
			"--record-version", version,
			NULL);

		perror("/usr/bin/telem-record-gen");
		exit(EXIT_FAILURE);
	} else {
		/* parent */
		pid_t w;
		int wstatus;

		close(fd);
		free(tmp);

		//FIXME timeout - don't idle forever here
		w = waitpid(pid, &wstatus, 0);
		if (w == -1) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		}

		if (WIFEXITED(wstatus) && (WEXITSTATUS(wstatus) == 0)) {
			unlink(filename);
		} else {
			fprintf(stderr, "Processing swupd telemetry record %s failed (%d)\n",
				filename, WEXITSTATUS(wstatus));
		}
	}
}

int main(void)
{
	DIR *dir;
	struct dirent *entry;

	get_creds();

	if (chdir(TELEMETRY_DIR)) {
		perror(TELEMETRY_DIR);
		exit(EXIT_FAILURE);
	}

	dir = opendir(TELEMETRY_DIR);
	if (!dir) {
		perror(TELEMETRY_DIR);
		exit(EXIT_FAILURE);
	}

	while (true) {
		entry = readdir(dir);
		if (!entry) {
			break;
		}
		if (entry->d_name[0] == '.') {
			continue;
		}
		deliver_payload(entry->d_name);
	}
	closedir(dir);

	exit(EXIT_SUCCESS);
}
