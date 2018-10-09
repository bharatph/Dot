#include <Dot/DotLooper.hpp>
#include <Dot/Reader.hpp>
#include <Dot/Dot.hpp>
#include <regex>

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
			//read the main socket in regular intervals
			const char *buffer = comm_read_text(dot->getSocket(), 1024);
			if (buffer == NULL)
			{
				log_err(TAG, "Client disconnected");
				shouldRun = false;
				fireEvent(DotEvent::DISCONNECTED);
				continue;
			}
			//compare read line with registered readers
			for (Reader *reader : registeredReaders)
			{
				std::regex reg(reader->getMessage());
				if (std::regex_match(buffer, reg))
				{
					reader->notify(buffer); //FIXEME send timestamp and string
				}
				else
					reader->notify("");
			}
		}
	});
}

void dot::DotLooper::stop()
{
	shouldRun = false;
}

void dot::DotLooper::registerReader(Reader &reader)
{
	registeredReaders.push_back(&reader);
}

dot::DotLooper::~DotLooper()
{
	runnerThread->join();
}
