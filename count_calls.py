import sys
import os


def run_commands(filename):
    f = open(filename,'r')
    counts = {}
    for line in f:
        l = line.split("|")
        utility = l[0]
        args = l[3]
        pre = l[1]
        post = l[2]
        count_filename = utility+"_counts.txt"
        os.system(pre) 
        command = "strace -o " + count_filename + " " + utility + " " + args
        os.system(command)
        print(command)
        os.system(post)
        count_calls(count_filename, counts)
        os.system("rm -f " + count_filename)
    f.close()
    return counts

def count_calls(filename, counts):
    LOCALE_ARCHIVE_LINE = "open(\"/usr/lib/locale/locale-archive\", O_RDONLY|O_CLOEXEC)"
    CLOSE_LOCALE_ARCHIVE_LINE = "close(3)"
    f = open(filename,'r')
    foundLocale = False
    foundStart = False
    for line in f:
        #print(line)
        if LOCALE_ARCHIVE_LINE in line:
            foundLocale = True
            continue
        if foundLocale and CLOSE_LOCALE_ARCHIVE_LINE in line:
            foundStart = True
            continue
        if foundStart:
            parse_line(line, counts)            
    f.close()

def parse_line(line, count_dict):
    loc = line.find("(")
    key = line[0:loc]
    print(key)
    if key in count_dict:
        count_dict[key] = count_dict[key] + 1
    else:
        count_dict[key] = 1    

if __name__ == "__main__":
    count_dict = run_commands(sys.argv[1])
    for call in count_dict:
        print(call + ": " + str(count_dict[call]))
