#!/bin/python

import json
import os
import subprocess
import argparse

APPS_FILE = "apps.json"
BUILD_IMAGE_NAME = "cxx_buildimg"
BASE_IMAGE_VERSION = "latest"
BUILD_FOLDER = ".build"

class App:
    def __init__(self, name, path):
        self.name = name
        self.path = path

def run_command(cmd):
    process = subprocess.Popen(cmd.split())
    output, error = process.communicate()

def read_apps():
    apps = []

    apps_file = open(APPS_FILE)
    apps_json = json.load(apps_file)
    for i in apps_json:
        name = i["name"]
        path = ""
        if "path" in i:
            path = "./" + BUILD_FOLDER + "/" + i["path"]
        else:
            path = "./" + BUILD_FOLDER + "/src/" + name + "/" + name

        app = App(name, path)
        apps.append(app)

    return apps

def build_targeted_app_list(app_list, all_apps):
    targeted = []
    for targeted_app in app_list:
        found = False
        for app in all_apps:
            if targeted_app == app.name :
                targeted.append(app)
                found = True
                break
        if found == False:
            print("Target " + targeted_app + " not found in " + APPS_FILE)

    return targeted 

def build_targets(targets):
    target_list = ""
    if not targets:
        target_list = "all"
        print("Building all targets")
    else:
        for target in targets:
            target_list += target.name + " "
        print("Building specific targets: " + target_list)

    pwd = os.getcwd()
    build_image = "docker build --rm --build-arg BASE_IMAGE_VERSION=" + BASE_IMAGE_VERSION + " -t " + BUILD_IMAGE_NAME + " . -f Dockerfile.build"
    
    run_build = "docker run --rm -v " + pwd + ":/source -it " + BUILD_IMAGE_NAME
    if targets is not None:
        run_build += " ./cmake-build.sh --target " + target_list

    remove_build_image = "docker rmi " + BUILD_IMAGE_NAME

    run_command(build_image)

    print("Running: " + run_build)
    run_command(run_build)
    run_command(remove_build_image)

def build_individual_image(app):
    copy_executable = "cp " + app.path + " ./app"
    build_image = "docker build --build-arg BASE_IMAGE_VERSION=" + BASE_IMAGE_VERSION + " -t " + app.name + ":latest ."
    remove_executable = "rm ./app"

    print("Building image for " + app.name)
    run_command(copy_executable)
    run_command(build_image)
    run_command(remove_executable)

def main():

    parser = argparse.ArgumentParser("Build docker images from C++ code")
    parser.add_argument('-t', '--targets', nargs='+', dest='targets', help='Targets to be built by CMake. Default is all', required=False)
    args = parser.parse_args()
    
    all_apps = read_apps()

    if args.targets:
        apps_targeted = build_targeted_app_list(args.targets, all_apps)
        build_targets(apps_targeted)
        for app in apps_targeted:
            build_individual_image(app)
    else:
        build_targets([])
        for app in all_apps:
            build_individual_image(app)

if __name__ == "__main__":
    main()
