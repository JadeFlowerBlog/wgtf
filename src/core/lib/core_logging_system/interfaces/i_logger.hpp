#ifndef I_LOGGER_HPP
#define I_LOGGER_HPP

namespace wgt
{
class LogMessage;

class ILogger
{
public:

	virtual void out( LogMessage* message ) = 0;
};

}
#endif // I_LOGGER_HPP
