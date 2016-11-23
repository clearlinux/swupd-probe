swupd-probe(1) -- Deliver swupd telemetry records
=================================================

## SYNOPSIS

`swupd-probe`
`/var/lib/swupd/telemetry`

## DESCRIPTION

`swupd-probe` is a telemetry delivery agent that relays telemetry
records from `swupd(1)` to `telemd(1)`. The telemetry records are
stored in `/var/lib/swupd/telemetry` and are not directly accessible
by `telemd`, hence the use of this agent.

## SYNTAX

The agent is not to be executed directly from the commandline or
directly by a user. A `swupd-probe.path` unit instructs systemd to
start the agent on demand whenever records are created.

## EXIT STATUS

The agent exits when there are no more records to relay to the
telemetry service. A non-zero exit status indicates a failure
occurred.

## COPYRIGHT

 * Copyright (C) 2016 Intel Corporation, License: CC-BY-SA-3.0

## SEE ALSO

 * swupd(1), telemd(1)

 * https://clearlinux.org/documentation/

## NOTES

Creative Commons Attribution-ShareAlike 3.0 Unported

 * http://creativecommons.org/licenses/by-sa/3.0/
