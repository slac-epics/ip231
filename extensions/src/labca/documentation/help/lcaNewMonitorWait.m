%
%lcaNewMonitorWait
%
%  Calling Syntax
%
%lcaNewMonitorValue(pvs, type)
%
%  Description
%
%   Similar to [1]lcaNewMonitorValue but instead of returning the status
%   of monitored PVs this routine blocks until all PVs have fresh data
%   available (e.g., due to initial connection or changes in value and/or
%   severity status). Reading the actual data must be done using
%   [2]lcaGet.
%
%  Parameters
%
%   pvs
%          Column vector (in matlab: m x 1 cell- matrix) of m strings.
%
%   type
%          (optional argument) A string specifying the data type to be
%          used for the channel access data transfer. The native type is
%          used by default. See [3]here for more information.
%
%          Note that monitors are specific to a particular data type and
%          therefore lcaNewMonitorWait will only report the status for a
%          monitor that had been established (by [4]lcaSetMonitor) with a
%          matching type. Using the ``native'' type, which is the default,
%          for both calls satisfies this condition.
%
%  Examples
%
%try lcaNewMonitorWait(pvs)
%        vals = lcaGet(pvs)
%catch
%        errs = lcaLastError()
%        handleErrors(errs)
%end
%     _________________________________________________________________
%
%
%    for EPICS management 2007-11-16
%
%References
%
%   1. lcaNewMonitorValue.html#lcanewmonitorvalue
%   2. lcaGet.html#lcaget
%   3. lcaGet.html#typearg
%   4. lcaSetMonitor.html#lcasetmonitor
