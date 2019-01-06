import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from functools import reduce
import operator
import seaborn as sns
import csv

def get_stats(path, number_of_nodes, bucket_size_step=100):
    data = pd.read_csv(path, delimiter=' ', header=None)
    data = data[data.iloc[:, 0] == number_of_nodes]
    data = data.reset_index(drop=True)
    data = data.drop(data.columns[0], axis=1)
    data = data.applymap(lambda x: 1 if x == 0 else x)
    nr_of_records = len(data)

    if bucket_size_step > nr_of_records:
        raise Exception('bucket_size_step should be not greater than nr_of_records')
    if nr_of_records % bucket_size_step != 0:
        raise Exception('nr_of_records_step should be divisible by bucket_size_step')

    number_of_buckets = int(np.ceil(nr_of_records / bucket_size_step))
    end_indexes = [bucket_size_step*i - 1 for i in range(1, number_of_buckets + 1)]
    avg_times = [data.iloc[data.index <= end_index].mean(axis=0) for end_index in end_indexes]
    diffs_in_avgs = [abs(100*(j - i)/i) for i, j in zip(avg_times[:-1], avg_times[1:])]

    return data.min().values[0], data.max().values[0], data.mean().values[0], \
           data.median().values[0], data.std().values[0], [x.values[0] for x in diffs_in_avgs], data, nr_of_records


cpp_time_data_path = './networks_cpp_time'
java_time_data_path = './networks_java_time'
number_of_nodes = [100, 250, 500, 750, 1000, 1500]
bucket_size_step = 200

cpp_results = []
java_results = []

for i in range(len(number_of_nodes)):
    cpp_results.append(get_stats(cpp_time_data_path, number_of_nodes[i], bucket_size_step))
    java_results.append(get_stats(java_time_data_path, number_of_nodes[i], bucket_size_step))

with open('./stats/diffs', 'w') as file:
    file_writer = csv.writer(file, delimiter=' ')
    file_writer.writerow(['nr_of_nodes', 'between_buckets', 'cpp_diffs', 'java_diffs'])
    for nr in number_of_nodes:
        index = number_of_nodes.index(nr)
        nr_cpp_results = cpp_results[index]
        nr_java_results = java_results[index]
        for j in range(len(nr_cpp_results[5])):
            file_writer.writerow([nr, str(int((j + 1)*bucket_size_step)) + '_' + str(int((j + 2)*bucket_size_step)),
                                  nr_cpp_results[5][j],
                                  nr_java_results[5][j]])

with open('./stats/statystyki', 'w') as file:
    file_writer = csv.writer(file, delimiter=' ')
    file_writer.writerow(['nr_of_nodes', 'cpp_min', 'java_min', 'cpp_max', 'java_max', 'cpp_mean', 'java_mean',
                          'cpp_median', 'java_median', 'cpp_std', 'java_std'])
    for nr in number_of_nodes:
        index = number_of_nodes.index(nr)
        nr_cpp_results = cpp_results[index]
        nr_java_results = java_results[index]
        file_writer.writerow([nr, nr_cpp_results[0],  nr_java_results[0], nr_cpp_results[1], nr_java_results[1],
                              nr_cpp_results[2], nr_java_results[2], nr_cpp_results[3], nr_java_results[3],
                              nr_cpp_results[4], nr_java_results[4]])

# index = number_of_nodes.index(750)
# nr_cpp_results = cpp_results[index]
# nr_java_results = java_results[index]
# print('cpp min: {} | java min: {}'.format(nr_cpp_results[0], nr_java_results[0]))
# print('cpp max: {} | java max: {}'.format(nr_cpp_results[1], nr_java_results[1]))
# print('cpp mean: {} | java mean: {}'.format(nr_cpp_results[2], nr_java_results[2]))
# print('cpp median: {} | java median: {}'.format(nr_cpp_results[3], nr_java_results[3]))
# print('cpp std: {} | java std: {}'.format(nr_cpp_results[4], nr_java_results[4]))

f = plt.figure()
plt.plot(number_of_nodes, [x[2] for x in cpp_results], 'bo', number_of_nodes, [x[2] for x in java_results], 'ro')
plt.xticks(number_of_nodes)
plt.title('Czas działania algorytmu dla 1000 uśrednień')
plt.xlabel('liczba wierchołków')
plt.ylabel('czas [ms]')
plt.legend(['C++ -O3', 'Java'])
plt.grid()
f.savefig('./stats/sredni_czas.pdf', bbox_inches='tight')
plt.show()

f = plt.figure()
plt.plot(number_of_nodes, np.log10([x[2] for x in cpp_results]), 'bo', number_of_nodes, np.log10([x[2] for x in java_results]), 'ro')
plt.xticks(number_of_nodes)
plt.title('Czas działania algorytmu w skali logarytmicznej o podstawie 10 dla 1000 uśrednień')
plt.xlabel('liczba wierchołków')
plt.ylabel('log10(czas/1ms)')
plt.legend(['C++ -O3', 'Java'])
plt.grid()
f.savefig('./stats/log_sredni_czas.pdf', bbox_inches='tight')
plt.show()

f = plt.figure()
plt.plot(number_of_nodes, [x[0][2] / x[1][2] for x in zip(java_results, cpp_results)], 'bo')
plt.xticks(number_of_nodes)
plt.title('Stosunek czasu działania Javy do C++ -O3 dla 1000 uśrednień')
plt.xlabel('liczba wierchołków')
plt.ylabel('stosunek')
plt.grid()
f.savefig('./stats/stosunek.pdf', bbox_inches='tight')
plt.show()

number_of_repetitions = cpp_results[0][7]
cpp_concat = pd.concat([x[6] for x in cpp_results])
java_concat = pd.concat([x[6] for x in java_results])
indexes = reduce(operator.concat, [[i]*number_of_repetitions for i in number_of_nodes])
cpp_concat['nr_of_nodes'] = indexes
cpp_concat['type'] = ['cpp' for _ in range(number_of_repetitions*len(number_of_nodes))]
java_concat['nr_of_nodes'] = indexes
java_concat['type'] = ['java' for _ in range(number_of_repetitions*len(number_of_nodes))]
concat = pd.concat([cpp_concat, java_concat], axis=0)
concat = concat.reset_index(drop=True)
concat.rename(columns={1: 'time'}, inplace=True)

# concat['time'] = concat['time'].apply(lambda x: np.log10(x) if x != 0 else 1)
concat['time'] = np.log10(concat['time'])
f = plt.figure()
ax = sns.boxplot(x='nr_of_nodes', y='time', hue='type', data=concat, palette='Set2')
plt.title('Wykresy pudełkowe dla 1000 uśrednień')
plt.xlabel('liczba wierchołków')
plt.ylabel('log10(czas/1ms)')
handles, _ = ax.get_legend_handles_labels()
ax.legend(handles, ['C++ -O3', 'Java'])
f.savefig('./stats/boxplot.pdf', bbox_inches='tight')
plt.show()
