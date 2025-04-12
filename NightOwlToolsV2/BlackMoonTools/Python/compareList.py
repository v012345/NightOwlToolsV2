import os
import csv


def read_list(list_path, to_data):
    if os.path.isfile(list_path):
        with open(list_path, 'r') as f:
            for row in csv.reader(f):
                to_data[row[2]] = row


if __name__ == "__main__":
    list1, list2 = {}, {}
    read_list("C:/Users/Meteor/Desktop/temp/list/list-v2_1.csv", list1)
    read_list("C:/Users/Meteor/Desktop/temp/list/list-v2_2.csv", list2)
    diff = []
    list1_has = []
    list2_has = []
    for i in list1:
        if not list2.get(i):
            list1_has.append(i)

    for i in list2:
        if not list1.get(i):
            list2_has.append(i)
        else:
            if list2[i][3] != list1[i][3]:
                diff.append(i)
    # print(list1_has)
    # print(list2_has)
    # print(diff)
    with open("temp/list1_has.txt", 'w', newline='', encoding='utf-8') as f:
                for v in list1_has:
                    f.write(v)
                    f.write("\n")
    with open("temp/list2_has.txt", 'w', newline='', encoding='utf-8') as f:
                for v in list2_has:
                    f.write(v)
                    f.write("\n")
    with open("temp/diff.txt", 'w', newline='', encoding='utf-8') as f:
                for v in diff:
                    f.write(v)
                    f.write("\n")