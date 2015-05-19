#ifndef I_ACTION_HPP
#define I_ACTION_HPP

class IAction
{
public:
	virtual ~IAction() {}

	virtual const char * text() const = 0;
	virtual bool enabled() const = 0;
	virtual void execute() = 0;
};

#endif//I_ACTION_HPP