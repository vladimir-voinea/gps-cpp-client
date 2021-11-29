#!/bin/python

import json
import os
import subprocess

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

def build_all():
    pwd = os.getcwd()
    build_image = "docker build --rm --build-arg BASE_IMAGE_VERSION=" + BASE_IMAGE_VERSION + " -t " + BUILD_IMAGE_NAME + " . -f Dockerfile.build"
    run_build = "docker run --rm -v " + pwd + ":/source -it " + BUILD_IMAGE_NAME
    remove_build_image = "docker rmi " + BUILD_IMAGE_NAME

    run_command(build_image)
    run_command(run_build)
    #run_command(remove_build_image)

def build_individual_image(app):
    copy_executable = "cp " + app.path + " ./app"
    build_image = "docker build --build-arg BASE_IMAGE_VERSION=" + BASE_IMAGE_VERSION + " -t " + app.name + ":latest ."
    remove_executable = "rm ./app"

    run_command(copy_executable)
    run_command(build_image)
    run_command(remove_executable)

def main():
    apps = read_apps()

    build_all()

    for app in apps:
        build_individual_image(app)


if __name__ == "__main__":
    main()
