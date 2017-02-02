import sys

def parseFile(filename):
    f = open(filename, "r")
    applications = f.read()
    application_list = applications.split('" "')
    apps = []
    for line in application_list:
        pcs = line.split(" ")
        apps.append(pcs[0])
    return apps

def fmt_print(app_list):
    app_list.sort()
    for a in app_list:
        print a

if __name__ == "__main__":
    app_list = parseFile(sys.argv[1])
    fmt_print(app_list)
