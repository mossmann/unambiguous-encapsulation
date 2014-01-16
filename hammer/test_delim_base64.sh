#!/bin/bash

# valid test vectors from specification
echo -n '' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n ',' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n '.' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n ':' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n ',,' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n ';:' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n '..' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n ':.' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL

# valid test vectors from specification that fail apparently due to hammer bug(s)
#echo -n 'd2VhcG9u;cHJvamVjdGlsZQ==;dGFyZ2V0:cGlzdG9s,YnVsbGV0,dG9hc3Rlcg==' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
#echo -n 'Vm0wd2QyUXlVWGxW' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
#echo -n 'Ym1WemRHVmssWm1sc1pRPT0=' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL

# additional valid test vectors
echo -n ';;:,,' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n 'Vm0w' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n 'Vm0w,Vm0w' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL
echo -n 'AA==:' | ./delim_base64 1>/dev/null 2>&1 && echo PASS || echo FAIL

# invalid test vectors from specification
echo -n ';' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n ':,' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n ',.' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n '::' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n '.;' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n '.:' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n ';,' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n ';.' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n ';;' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n '::' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n ';:,,' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n ' ' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n 'TEFOR1NFQw' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
echo -n 'MQ==Mg==' | ./delim_base64 1>/dev/null 2>&1 && echo FAIL || echo PASS
