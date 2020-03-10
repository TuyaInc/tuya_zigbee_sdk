# coding:utf-8

import os

def searchFile(root, target):
    items = os.listdir(root)
    for item in items:
        path = os.path.join(root, item)
        if os.path.isdir(path):
            searchFile(path, target)
        elif path.split('/')[-1] == target:
            root_dir = os.getcwd()
            target_dir = path.replace(target, '')
            print( '[project]' + target_dir)
            os.chdir(target_dir)
            os.getcwd()
            os.system("./" + target + " clean ")
            os.system("./" + target + " build ")
            os.chdir(root_dir)
        else:
            pass

if __name__ == '__main__':
    searchFile('./', 'run.sh')
