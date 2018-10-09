#include <Dot/DotLooper.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Dot.hpp>
#include <regex>
#include <fstream>

extern "C"
{
#include <clog/clog.h>
#include <comm.h>
}

#include <Dot/DotEvent.hpp>

static const char *TAG = "DotLooper";

dot::DotLooper::DotLooper()
{
}
dot::DotLooper::DotLooper(Dot *dot) : dot(dot)
{
}

//use libev insted to read
void dot::DotLooper::run()
{
	runnerThread = new std::thread([&]() {
		shouldRun = true;
		while (shouldRun)
		{
			if (textReaders.size() == 0)
			{
				continue;
			}
			//read the main socket in regular intervals
			char *buffer = comm_read_text(dot->getSocket(), 1024);
			if (buffer == NULL)
			{
				log_inf(TAG, "Client disconnected");
				shouldRun = false;
				fireEvent(DotEvent::DISCONNECTED);
				continue;
			}
			//compare read line with registered readers
			for (std::pair<Reader *, std::string> textReaderPair : textReaders)
			{
				std::regex reg(textReaderPair.second);
				if (std::regex_match(buffer, reg))
				{
					textReaderPair.first->notify(buffer); //FIXEME send timestamp and string
				}
				else
					textReaderPair.first->notify("");
			}
		}
	});
}

void dot::DotLooper::stop()
{
	shouldRun = false;
}

int dot::DotLooper::sendFile(std::string fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (file.fail())
	{
		return -1;
	}
	char *buffer = (char *)malloc(200);
	file.read(buffer, 200);
	if (buffer == NULL)
	{
		return -1;
	}
	int fileSize = strlen(buffer);
	//this->stop();
	std::cout << "file sent" << std::endl;
	return ::send(dot->getSocket(), buffer, fileSize, 0);
}

std::ofstream &dot::DotLooper::readFile(int toRead)
{
	std::ofstream &file = *(new std::ofstream());
	file.open("./dotFile");
	char *buffer = (char *)malloc(toRead);
	std::cout << "file reading from client" << std::endl;
	//this->stop();
	::recv(dot->getSocket(), buffer, toRead, 0);
	file.write(buffer, toRead);
	return file;
}

void dot::DotLooper::registerReader(Reader &reader, std::string message)
{
	textReaders[&reader] = message;
}

dot::DotLooper::~DotLooper()
{
	runnerThread->join();
}
