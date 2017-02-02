import sys
import os


def run_commands(filename):
    f = open(filename,'r')
    failed_calls = []
    counts = {}
    for line in f:
        l = line.split("|")
        utility = l[0]
        if "#" in utility:
            continue
        print("Utility: " + utility)
        args = l[3]
        pre = l[1]
        post = l[2]
        count_filename = utility+"_counts.txt"
        os.system(pre) 
        command = "ltrace -o " + count_filename + " " + utility + " " + args
        os.system(command)
        print(command)
        os.system(post)
        count_calls(count_filename, counts, failed_calls)
        os.system("rm -f " + count_filename)
    f.close()

    failed_file = open("failed_calls.txt",'w')
    for call in failed_calls:
        failed_file.write(call + "\n")
    failed_file.close()
    return counts

def count_calls(filename, counts, failed_calls):
    LOCALE_ARCHIVE_LINE = "open(\"/usr/lib/locale/locale-archive\", O_RDONLY|O_CLOEXEC)"
    CLOSE_LOCALE_ARCHIVE_LINE = "close("
    f = open(filename,'r')
    foundLocale = False
    foundStart = False
    fd = ""
    for line in f:
        print(line)
        if LOCALE_ARCHIVE_LINE in line:
            parts = line.split('=')
            fd = int(parts[1])
            foundLocale = True
            continue
        if foundLocale and (CLOSE_LOCALE_ARCHIVE_LINE + str(fd) + ")")  in line:
            foundStart = True
            continue
        if foundStart:
            parse_line(line, counts)            
    if not foundStart:
        failed_calls.append(filename)
        print("Start not found " + filename)
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
    f = open("combined_call_counts.txt", "w")
    for key, value in sorted(count_dict.iteritems(), key=lambda (k,v): (v,k)):
        f.write("%s: %s\n" % (key, int(value)))
    f.close()
