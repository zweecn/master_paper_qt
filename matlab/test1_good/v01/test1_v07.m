markov = importdata('test1\\mout.txt');
greedy = importdata('test1\\gout.txt');

t_markov = markov(1,:);
action_markov = markov(2,:);
reward_markov = markov(3,:) * 20;
price_markov = markov(4,:) * 20;
time_markov = markov(5,:) * 20;

t_greedy = greedy(1,:);
action_greedy = greedy(2,:);
reward_greedy = greedy(3,:) * 20;
price_greedy = greedy(4,:) * 20;
time_greedy = greedy(5,:) * 20;

m_action = importdata('test1\\m_action.txt');
g_action = importdata('test1\\g_action.txt');

t1 = m_action(1,:);
a1 = m_action(2,:);
t2 = g_action(1,:);
a2 = g_action(2,:);

t_markov = t1;
t_greedy = t2;

% subplot(2, 2, 1);
% plot(t1, a1, 'r-*', t2, a2, 'b:o');
% xlabel('t');
% ylabel('Action');
% legend('Markov', 'Greedy');
% x_size = (max(max(t1), max(t2)))+2;
% axis([-2, x_size, 0, max(max(a1), max(a2))+2]);

subplot(1, 3, 1);
% subplot(3, 1, 1);
plot(t_markov, reward_markov, 'r-o', t_greedy, reward_greedy, 'b:o', 'MarkerSize', 5);
xlabel('Time');
ylabel('Reward');
legend('Markov', 'Greedy');
y_size = min(min(reward_markov), min(reward_greedy)) - 10;
% axis([-2, x_size , y_size, 10]);

subplot(1, 3, 2);
% subplot(3, 1, 2);
plot(t_markov, price_markov, 'r-o', t_greedy, price_greedy, 'b:o', 'MarkerSize', 5);
xlabel('Time');
ylabel('Cost Overflow');
legend('Markov', 'Greedy');
% x_size = (max(max(t1), max(t2)))+2;
y_size = min(min(price_markov), min(price_greedy)) - 10;
% axis([-2, x_size , y_size, 10]);

subplot(1, 3, 3);
% subplot(3, 1, 3);
plot(t_markov, time_markov, 'r-o', t_greedy, time_greedy, 'b:o', 'MarkerSize', 5);
xlabel('Time');
ylabel('Time Delay');
legend('Markov', 'Greedy');
% x_size = (max(max(t1), max(t2)))+2;
y_size = min(min(time_markov), min(time_greedy)) - 10;
% axis([-2, x_size , y_size, 10]);