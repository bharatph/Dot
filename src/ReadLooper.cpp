#include <Dot/ReadLooper.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Dot.hpp>
#include <regex>

extern "C"
{
#include <clog/clog.h>
#include <comm.h>
}

static const char *TAG = "ReadLooper";

dot::ReadLooper::ReadLooper()
{
}
dot::ReadLooper::ReadLooper(Dot *dot) : dot(dot)
{
}

//use libev insted to read
void dot::ReadLooper::run()
{
	runnerThread = new std::thread([&]() {
		shouldRun = true;
		while (shouldRun)
		{
			//read the main socket in regular intervals
			const char *buffer = comm_read_text(dot->getSocket(), 1024);
			if (buffer == NULL)
			{
				log_err(TAG, "Error reading");
				shouldRun = false;
			}

			//compare read line with registered readers
			for (Reader *reader : registeredReaders)
			{
				std::string readMessage(buffer);
				std::regex reg(reader->getMessage());
				if (std::regex_match(readMessage, reg))
				{
					reader->notify(buffer); //FIXEME send timestamp and string
				}
			}
		}
	});
}

void dot::ReadLooper::stop()
{
	shouldRun = false;
}

void dot::ReadLooper::registerReader(Reader &reader)
{
	registeredReaders.push_back(&reader);
}

dot::ReadLooper::~ReadLooper()
{
	runnerThread->join();
}
