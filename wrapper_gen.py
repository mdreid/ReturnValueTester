import sys

def main():
	file = open(sys.argv[1])
	found_synopsis = False
	for line in file:
		if 'SYNOPSIS' in line:
			found_synopsis = True
			continue
		if found_synopsis and 'DESCRIPTION' in line:
			break
		elif found_synopsis and line != '\n':
			print line.strip()
		else:
			continue

if __name__ == '__main__':
	main()
