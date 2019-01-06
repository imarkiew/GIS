import pandas as pd
import numpy as np

def get_stats(path, number_of_nodes, bucket_size_step=100):
    data = pd.read_csv(path, delimiter=' ', header=None)
    data = data[data.iloc[:, 0] == number_of_nodes]
    data = data.drop(data.columns[0], axis=1)
    data = data.reset_index(drop=True)
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
           data.median().values[0], data.std().values[0], [x.values[0] for x in diffs_in_avgs]


cpp_time_data_path = './networks_cpp_time'
java_time_data_path = './networks_java_time'
number_of_nodes = 1000
bucket_size_step = 200

cpp_results = get_stats(cpp_time_data_path, number_of_nodes, bucket_size_step)
java_results = get_stats(java_time_data_path, number_of_nodes, bucket_size_step)

for i in range(len(cpp_results[5])):
    print('cpp avg diff: {} % | java avg diff: {} %'.format(cpp_results[5][i], java_results[5][i]))

print('cpp min: {} | java min: {}'.format(cpp_results[0], java_results[0]))
print('cpp max: {} | java max: {}'.format(cpp_results[1], java_results[1]))
print('cpp mean: {} | java mean: {}'.format(cpp_results[2], java_results[2]))
print('cpp median: {} | java median: {}'.format(cpp_results[3], java_results[3]))
print('cpp std: {} | java std: {}'.format(cpp_results[4], java_results[4]))