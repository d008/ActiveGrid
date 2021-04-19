staticIP = '128.112.33.23'
ports = 8080;
numPhotons = 1;
tcps = cell(numPhotons,1);
for i = 1:numPhotons
    tcps{i} = tcpip('0.0.0.0',8080, 'NetworkRole','server');
    fopen(tcps{i})
    fprintf('Port Open')
end

ticaa
N =1000;
for i = 1:N
    fread(tcps{1},1);
end
t = toc
N/t
fclose(tcps{1})
