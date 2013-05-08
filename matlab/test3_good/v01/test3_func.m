function [ output_args ] = test3_func( )
% do test2 plot

markov = importdata('test3\\markov.txt');
greedy = importdata('test3\\greedy.txt');
x0 = markov(:,1);
a0 = markov(:,2);
r0 = markov(:,3);
x1 = greedy(:,1);
a1 = greedy(:,2);
r1 = greedy(:,3);

ay = [a0,a1];

subplot(2, 1, 1);
name = {'', 'Continue', 'Terminate', 'Retry', 'Substitute', 'Re-compose', ''};
xname = {'0', '0.2', '0.4', '0.6', '0.8', '1', '1.2', '1.4', '1.6', '1.8', '2'};
b = bar(a0);
ch = get(b, 'children');
set(gca, 'YTickLabel', name);
set(gca, 'XTickLabel', xname);
xlabel('fc / total price of SPlan');
axis([0, 20, 0, 6]);

subplot(2, 1, 2);
plot(x0, r0, 'r-o', 'MarkerSize', 5);
xlabel('fc / total price of SPlan');
ylabel('Reward');

% plot(x0, a0, 'r-*', x1, a1, 'b-o');
% xlabel('Punishment / Total Price');
% ylabel('Action');
% legend('Markov', 'Greedy');
% axis([0, 2 , 0, 6]);
% text(0, 2, 'Stop')
% text(0, 3, 'Retry')
% text(0, 4, 'Substitute')
% text(0, 5, 'Re-composite')
% subplot(2, 1, 2);
% plot(x0, r0, 'r-*', x1, r1, 'b-o');
% xlabel('Punishment / Total Price');
% ylabel('Reward');
% legend('Markov', 'Greedy');

