markov = importdata('mout.txt');
greedy = importdata('gout.txt');

t_markov = markov(1,:);
action_markov = markov(2,:);
reward_markov = markov(3,:);
price_markov = markov(4,:) * (-1);
time_markov = markov(5,:)* (-1);

t_greedy = greedy(1,:);
action_greedy = greedy(2,:);
reward_greedy = greedy(3,:);
price_greedy = greedy(4,:)* (-1);
time_greedy = greedy(5,:)* (-1);

m_action = importdata('m_action.txt');
g_action = importdata('g_action.txt');

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

h1=subplot(1, 3, 1, 'replace');
po1=get(h1,'position');
plot(t_markov, reward_markov, 'r-o', t_greedy, reward_greedy, 'b-*', 'MarkerSize', 4);
xlabel('Time (s)');
ylabel('Reward ($)');
legend('Markov', 'Greedy');
y_size = min(min(reward_markov), min(reward_greedy)) - 10;
% axis([-2, x_size , y_size, 10]);

subplot(1, 3, 2);
po2=get(h1,'position');
% subplot(3, 1, 2);
plot(t_markov, price_markov, 'r-o', t_greedy, price_greedy, 'b-*', 'MarkerSize', 4);
xlabel('Time (s)');
ylabel('Cost Overflow ($)');
legend('Markov', 'Greedy');
% x_size = (max(max(t1), max(t2)))+2;
y_size = min(min(price_markov), min(price_greedy)) - 10;
% axis([-2, x_size , y_size, 10]);

subplot(1, 3, 3);
po3=get(h1,'position');
% subplot(3, 1, 3);
plot(t_markov, time_markov, 'r-o', t_greedy, time_greedy, 'b-*', 'MarkerSize', 4);
xlabel('Time (s)');
ylabel('Time Delay (s)');
legend('Markov', 'Greedy');
% x_size = (max(max(t1), max(t2)))+2;
y_size = min(min(time_markov), min(time_greedy)) - 10;
% axis([-2, x_size , y_size, 10]);