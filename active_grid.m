close all; 

% create cell array of tcpip information for each winglet
paddles = [11,31,51,71]; % each of these numbers correspond to one winglet
n=length(paddles);
tcps=cell(n,1);
for i=1:n
    t = tcpip(strcat('192.168.0.',num2str(100+paddles(i))),80);
    t.Terminator = "cr/lf"; 
    tcps{i} = t; 
end

% open each port 
for i=1:n
    fopen(tcps{i});
    disp(paddles(i));
end 
fprintf('All Ports Open!'); 

% send data to each paddle
for j = 1:n
    % this is the homing protocol 
    request = 'H';
    fprintf(tcps{i},request); 
    % send request in form of string
    request = 'R1D400S360T1'; 
    fprintf(tcps{j},request);
    % R# - rotation direction; # = 0 or 1
    % D#### - delay in microseconds (controls speed)
    % S### - step ### degrees 
    % T## - repeats the protocol for ## times 
    pause(0.03) % pause between each winglet data for staggered movement
end

% close each port 
for i=1:n
    fclose(tcps{i});
    disp(i);
end 
fprintf('All Ports Closed!'); 