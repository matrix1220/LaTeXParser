#pragma once
#include "includes.h"
#include "Tokenizer.h"

namespace LaTeX
{
namespace PRSR
{

class Content;
class Contents: public std::vector<Content> {
public:
	std::string toString(int i=0) const;
	using std::vector<Content>::vector;
};

class Environment {
public:
	virtual std::string toString(int i=0) const = 0;
};

enum class HandlerResult {
	NOT_FOR_ME,
	GOOD
};

//using HandlerEndResult = std::variant<Content, EnvironmentHandler*>;

enum class HandlerState {
	INVALID,
	ENDED,
	READY_TO_READ,
	READING_ENVIRONMENT
};


class DefaultEnvironment: public Environment {
public:
	Contents content;
	DefaultEnvironment();
	DefaultEnvironment(Contents content);
	std::string toString(int i=0) const override;
};
class NamedDefaultEnvironment: public Environment {
public:
	std::string name;
	Contents content;
	NamedDefaultEnvironment();
	NamedDefaultEnvironment(std::string name, Contents content);
	std::string toString(int i=0) const override;
};

class CurlyBracketEnvironment: public Environment
{
public:
	Contents content;
	std::string toString(int i=0) const override;
};

class DefaultCommand: public Environment
{
public:
	std::string name;
	Contents content; //CurlyBracketEnvironment
	DefaultCommand();
	DefaultCommand(std::string name, Contents content);
	std::string toString(int i=0) const override;
};

class SimpleDefaultCommand: public Environment
{
public:
	std::string name;
	std::string content;
	SimpleDefaultCommand();
	SimpleDefaultCommand(std::string name, std::string content);
	std::string toString(int i=0) const override;
};

class Content {
public:
	std::variant<
		TKNZR::Content,
		DefaultEnvironment,
		NamedDefaultEnvironment,
		CurlyBracketEnvironment,
		DefaultCommand,
		SimpleDefaultCommand
	> content;
	template <class T>
	Content(T content);
	Content();
	//~Content();
	void operator=(const Content& temp);
	std::string toString(int i=0) const;
};

// handlers

class EnvironmentHandler {
public:
	EnvironmentHandler* environmentHandler = nullptr;
	HandlerState state = HandlerState::INVALID;
	virtual HandlerResult handle(Content) = 0;
	virtual HandlerResult activate(Content) = 0;
	virtual Content end() = 0;
};

class DefaultEnvironmentHandler: public EnvironmentHandler
{
public:
	DefaultEnvironment root;
	std::vector<EnvironmentHandler*> stackOfEnvironmentHandlers;
	DefaultEnvironmentHandler();
	HandlerResult handle(Content) override;
	HandlerResult activate(Content) override;
	Content end() override;

	template<class Type>
	EnvironmentHandler* activator(Content content);
};

class NamedDefaultEnvironmentHandler: public EnvironmentHandler
{
public:
	NamedDefaultEnvironment root;
	NamedDefaultEnvironmentHandler();
	HandlerResult handle(Content) override;
	HandlerResult activate(Content) override;
	Content end() override;
};

//class BracketEnvironmentHandler: public EnvironmentHandler
class CurlyBracketEnvironmentHandler: public EnvironmentHandler
{
public:
	CurlyBracketEnvironment root;
	HandlerResult handle(Content) override;
	HandlerResult activate(Content) override;
	Content end() override;
};


class DefaultCommandHandler: public EnvironmentHandler
{

public:
	DefaultCommand root;
	DefaultCommandHandler();
	HandlerResult handle(Content) override;
	HandlerResult activate(Content) override;
	Content end() override;
};
class SimpleDefaultCommandHandler: public EnvironmentHandler
{
public:
	SimpleDefaultCommand root;
	HandlerResult handle(Content) override;
	HandlerResult activate(Content) override;
	Content end() override;
};


Content Parse(TKNZR::Tokenizer& stream);

}
}