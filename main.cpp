/*
 * Create a simple console or GUI application
 * which visualizes the process tree of the operating system
 * (similar to pstree command) using procfs.
 * The application should support auto refresh.
 * Target operating system is Linux.
 *
 */


#include <iostream>
#include <fstream>
#include <glob.h>
#include <map>
#include <set>
#include <memory>
#include <regex>

typedef int ID;
#define MAX_DEPTH 3

struct Proc
{
	ID pid;
	ID ppid;
};

class Processes
{
public:

	int GetProcesses(void);
	int PrintProcTree(unsigned depth);

public:
	std::map< ID, Proc* > procMap;
};


int Processes::GetProcesses(void)
{
	/* generate pathnames matching a pattern /proc/[0-9]* */

	glob_t glob_result;
	unsigned int i, j;

	int err = glob("/proc/[0-9]*", GLOB_NOSORT, NULL, &glob_result);

	if(err == 0)
	{
		for (size_t i = 0; i < glob_result.gl_pathc; i++)
		{
			Proc* proc;

			std::cout << glob_result.gl_pathv[i] << std::endl;

			std::string path_to_stat = std::string(glob_result.gl_pathv[i]) + "/stat";

			// todo: check if exists

			std::ifstream stat_file;
			stat_file.open(path_to_stat);

			if(stat_file.is_open())
			{
				std::string stat_line;
				int counter = 0;
				proc = new Proc();

				// todo: re-implement with regex!
				while(std::getline(stat_file, stat_line, ' ') && (counter++ <= 4) )
				{
					if(counter == 1)
					{
						proc->pid = std::stoi(stat_line);
						continue;
					}

					if(counter == 4)
					{
						proc->ppid = std::stoi(stat_line);
						break;
					}
				}
			}
			else
			{
				throw std::runtime_error("can't read file");
			}

			procMap[proc->pid] = proc;

		}

		globfree(&glob_result);
	}
	else
	{
		// todo: add error processing
		std::cerr << "glob function have returned error: " << err << std::endl;
	}
}

int Processes::PrintProcTree(unsigned depth)
{
	// todo: print as tree
	for(std::map<ID, Proc*>::iterator iterator = procMap.begin(); iterator != procMap.end(); ++iterator)
	{
		std::cout << iterator->first << ": " << iterator->second->ppid << std::endl;
	}
}

int main()
{
	// todo: add auto refresh

	try
	{
		std::unique_ptr<Processes> os_proc = std::unique_ptr<Processes>(new Processes());

		/* Get all processes of OS */
		os_proc->GetProcesses();

		/* Remove zombies (processes with PID 0 */
		// TODO!

		/* Print process tree (by ID) */
		os_proc->PrintProcTree(MAX_DEPTH);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
