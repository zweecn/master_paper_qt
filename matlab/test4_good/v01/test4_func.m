function [ output_args ] = Untitled1( )
% do test4 plot
data1 = importdata('test4\\out1.txt');
data2 = importdata('test4\\out2.txt');
data = (data1 + data2) / 2;
% serial_x = data(1,:);
% serial_y = data(2,:);
% parallel_x = data(3,:);
% parallel_y = data(4,:);
% parallel_serial_x = data(5,:);
% parallel_serial_y = data(6,:);

x1 = data(1,:);
y1 = data(2,:);
x2 = data(3,:);
y2 = data(4,:);
x3 = data(5,:);
y3 = data(6,:);
 
serial_x = [1:8];
serial_y = [1:8];
parallel_x = [1:8];
parallel_y = [1:8];
parallel_serial_x = [1:8];
parallel_serial_y = [1:8];

for i=1:8
    serial_x(i) = x1(i*10);
    serial_y(i) = y1(i*10);
    parallel_x(i) = x2(i*10);
    parallel_y(i) = y2(i*10);
    parallel_serial_x(i) = x3(i*10);
    parallel_serial_y(i) = y3(i*10);
end

plot(serial_x, serial_y, 'r-o', parallel_x, parallel_y, 'b-^', parallel_serial_x, parallel_serial_y, 'k-*');
xlabel('Number of activities');
ylabel('Execution time (ms)');
legend('Serial', 'Parallel', 'Complex', 'Location','NorthWest');