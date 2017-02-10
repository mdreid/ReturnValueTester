import sys
import os


def lib_counts(filename, counts):
    f = open(filename, 'r')    
    for line in f:
        #print(line)
        counts = parse_line(line, counts)            
    f.close()
    return counts

def run_commands(filename, cmd_type):
    f = open(filename,'r')
    counts = {}
    for line in f:
        if "#" in line:
            continue
        l = line.split("|")
        utility = l[0]
        args = l[3]
        pre = l[1]
        post = l[2]
        count_filename = utility+"_counts.txt"
         
        os.system(pre)
                
        if (cmd_type == "-s"):
            command = "strace -o " 
        else:
            command = "ltrace -o "
        command = command + count_filename + " " + utility + " " + args
        os.system(command)
        #print(utility + " " + args + " > " + count_filename)
        if (cmd_type == "-s"):
            counts = count_calls(count_filename, counts)
        else:
            counts = lib_counts(count_filename, counts)            
        os.system(post)
        os.system("rm -f " + count_filename)
    f.close()
    #print(counts)
    return counts

def count_calls(filename, counts):
    LOCALE_ARCHIVE_LINE = "open(\"/usr/lib/locale/locale-archive\", O_RDONLY|O_CLOEXEC)"
    CLOSE_LOCALE_ARCHIVE_LINE = "close("
    f = open(filename,'r')
    foundLocale = False
    foundStart = False
    for line in f:
        #print(line)
        if LOCALE_ARCHIVE_LINE in line:
            foundLocale = True
            continue
        if foundLocale and (CLOSE_LOCALE_ARCHIVE_LINE in line):
            foundStart = True
            continue
        if foundStart:
            counts = parse_line(line, counts)            
    f.close()
    return counts

def parse_line(line, count_dict):
    loc = line.find("(")
    key = line[0:loc]
    if key in count_dict:
        count_dict[key] = count_dict[key] + 1
    else:
        count_dict[key] = 1    
    return count_dict

if __name__ == "__main__":
    count_dict = run_commands(sys.argv[1], sys.argv[2])
    f = open("count" + sys.argv[2] + "-results.txt","w+")
    for call in count_dict:
        print(call + ": " + str(count_dict[call]))
        f.write(call + ": " + str(count_dict[call]) + "\n")
    f.close()
