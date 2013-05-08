data = [3, 7, 5, 2; 2 3 1 3];
b = bar(data);
ch = get(b, 'children');
set(ch, 'FaceVertexCData',[0 0 1;0 1 1;1 1 1;1 0 1;])