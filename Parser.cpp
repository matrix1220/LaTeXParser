#include "Parser.h"

namespace LaTeX
{
namespace PRSR
{
// Low level
DefaultEnvironmentHandler::DefaultEnvironmentHandler() {}
template<class Type>
EnvironmentHandler* DefaultEnvironmentHandler::activator(Content content) {
	EnvironmentHandler* environmentHandler = new Type();
	if(environmentHandler->activate(content)==HandlerResult::GOOD) return environmentHandler;
	delete environmentHandler;
	return nullptr;
}
HandlerResult DefaultEnvironmentHandler::activate(Content content) {
	if(state != HandlerState::INVALID) return HandlerResult::NOT_FOR_ME;
	state = HandlerState::READY_TO_READ;
	handle(content);
	return HandlerResult::GOOD;
}
HandlerResult DefaultEnvironmentHandler::handle(Content content) {
	environmentHandler = activator<DefaultCommandHandler>(content);
	if(environmentHandler!=nullptr) {
		stackOfEnvironmentHandlers.push_back(environmentHandler);
		return HandlerResult::GOOD;
	}
	environmentHandler = activator<CurlyBracketEnvironmentHandler>(content);
	if(environmentHandler!=nullptr) {
		stackOfEnvironmentHandlers.push_back(environmentHandler);
		return HandlerResult::GOOD;
	}
	environmentHandler = activator<SimpleDefaultCommandHandler>(content);
	if(environmentHandler!=nullptr) {
		stackOfEnvironmentHandlers.push_back(environmentHandler);
		return HandlerResult::GOOD;
	}
	environmentHandler = activator<NamedDefaultEnvironmentHandler>(content);
	if(environmentHandler!=nullptr) {
		stackOfEnvironmentHandlers.push_back(environmentHandler);
		return HandlerResult::GOOD;
	}
	if(stackOfEnvironmentHandlers.empty()) {
		//std::cout << content.toString() << "\n";
		root.content.push_back(content);
	} else {
		HandlerResult temp = stackOfEnvironmentHandlers.back()->handle(content);
		if(temp==HandlerResult::NOT_FOR_ME) {
			Content tempContent = stackOfEnvironmentHandlers.back()->end();
			//std::cout << tempContent.toString() << "\n";
			delete stackOfEnvironmentHandlers.back();
			stackOfEnvironmentHandlers.pop_back();
			while(handle(tempContent)==HandlerResult::NOT_FOR_ME);
			return HandlerResult::NOT_FOR_ME;
		}
	}
	return HandlerResult::GOOD;
};
Content DefaultEnvironmentHandler::end() {
	while(!stackOfEnvironmentHandlers.empty()) {
		Content tempContent = stackOfEnvironmentHandlers.back()->end();
		delete stackOfEnvironmentHandlers.back();
		stackOfEnvironmentHandlers.pop_back();
		while(handle(tempContent)==HandlerResult::NOT_FOR_ME);
	}
	state = HandlerState::INVALID;
	return Content(root);
}

NamedDefaultEnvironmentHandler::NamedDefaultEnvironmentHandler() {}
HandlerResult NamedDefaultEnvironmentHandler::activate(Content content) {
	if(state != HandlerState::INVALID) return HandlerResult::NOT_FOR_ME;
	if(!std::holds_alternative<SimpleDefaultCommand>(content.content)) return HandlerResult::NOT_FOR_ME;
	SimpleDefaultCommand& tempCommand = std::get<SimpleDefaultCommand>(content.content);
	if(tempCommand.name != "begin") return HandlerResult::NOT_FOR_ME;
	root.name = tempCommand.content;
	state = HandlerState::READY_TO_READ;
	return HandlerResult::GOOD;
}
HandlerResult NamedDefaultEnvironmentHandler::handle(Content content) {
	if(state == HandlerState::READY_TO_READ) {
		if(std::holds_alternative<SimpleDefaultCommand>(content.content)) {
			SimpleDefaultCommand& tempCommand = std::get<SimpleDefaultCommand>(content.content);
			if(tempCommand.name=="end" and root.name==tempCommand.content) {
				state = HandlerState::ENDED;
				return HandlerResult::GOOD;
			}
		}
		root.content.push_back(content);
		return HandlerResult::GOOD;
	} 
	return HandlerResult::NOT_FOR_ME;
};
Content NamedDefaultEnvironmentHandler::end() {
	state = HandlerState::INVALID;
	return Content(root);
}

DefaultCommandHandler::DefaultCommandHandler() {}
HandlerResult DefaultCommandHandler::activate(Content temp) {
	if(state != HandlerState::INVALID) return HandlerResult::NOT_FOR_ME;
	if(!std::holds_alternative<TKNZR::Content>(temp.content)) return HandlerResult::NOT_FOR_ME;
	TKNZR::Content& content = std::get<TKNZR::Content>(temp.content);

	if(content.type!=TKNZR::Type::Command) return HandlerResult::NOT_FOR_ME;
	root.name = content.content;
	state = HandlerState::READY_TO_READ;
	//std::cout << temp.toString() << "\n";
	return HandlerResult::GOOD;
}
HandlerResult DefaultCommandHandler::handle(Content content) {
	//std::cout << content.toString() << "\n";
	if(state == HandlerState::READY_TO_READ) {
		state = HandlerState::ENDED;
		if(std::holds_alternative<CurlyBracketEnvironment>(content.content)) {
			root.content = std::get<CurlyBracketEnvironment>(content.content).content;
			return HandlerResult::GOOD;
		}
	}
	return HandlerResult::NOT_FOR_ME;
};
Content DefaultCommandHandler::end() {
	state = HandlerState::INVALID;
	return Content(root);
}

HandlerResult SimpleDefaultCommandHandler::activate(Content temp) {
	if(state != HandlerState::INVALID) return HandlerResult::NOT_FOR_ME;
	if(!std::holds_alternative<DefaultCommand>(temp.content)) return HandlerResult::NOT_FOR_ME;
	DefaultCommand& tempCommand = std::get<DefaultCommand>(temp.content);
	if(tempCommand.content.size()!=1) return HandlerResult::NOT_FOR_ME;
	if(!std::holds_alternative<TKNZR::Content>(tempCommand.content.front().content)) return HandlerResult::NOT_FOR_ME;
	TKNZR::Content& tempContent = std::get<TKNZR::Content>(tempCommand.content.front().content);
	if(tempContent.type!=TKNZR::Type::Text) return HandlerResult::NOT_FOR_ME;
	root.name=tempCommand.name;
	root.content=tempContent.content;
	state = HandlerState::ENDED;
	return HandlerResult::GOOD;
}
HandlerResult SimpleDefaultCommandHandler::handle(Content temp) {
	return HandlerResult::NOT_FOR_ME;
}
Content SimpleDefaultCommandHandler::end() {
	return Content(root);
}

//CurlyBracketEnvironmentHandler::CurlyBracketEnvironmentHandler() {}
HandlerResult CurlyBracketEnvironmentHandler::activate(Content temp) {
	if(state != HandlerState::INVALID) return HandlerResult::NOT_FOR_ME;
	if(!std::holds_alternative<TKNZR::Content>(temp.content)) return HandlerResult::NOT_FOR_ME;
	TKNZR::Content& content = std::get<TKNZR::Content>(temp.content);

	if(content.type!=TKNZR::Type::Token or content.content!="{") return HandlerResult::NOT_FOR_ME;
	state = HandlerState::READY_TO_READ;
	return HandlerResult::GOOD;
}
HandlerResult CurlyBracketEnvironmentHandler::handle(Content temp) {
	if(state == HandlerState::READY_TO_READ) {
		if(std::holds_alternative<TKNZR::Content>(temp.content)) {
			TKNZR::Content& content = std::get<TKNZR::Content>(temp.content);
			if(content.type==TKNZR::Type::Token and content.content=="}") {
				state = HandlerState::ENDED;
				return HandlerResult::GOOD;
			}
		}
		root.content.push_back(temp);
		return HandlerResult::GOOD;
	} 
	return HandlerResult::NOT_FOR_ME;
};
Content CurlyBracketEnvironmentHandler::end() {
	state = HandlerState::INVALID;
	return Content(root);
}


// Environments
DefaultEnvironment::DefaultEnvironment() {}
DefaultEnvironment::DefaultEnvironment(Contents content): content(content) {}
std::string DefaultEnvironment::toString(int i) const {
	return content.toString(i);
}

NamedDefaultEnvironment::NamedDefaultEnvironment() {}
NamedDefaultEnvironment::NamedDefaultEnvironment(std::string name, Contents content): name(name), content(content) {}
std::string NamedDefaultEnvironment::toString(int i) const {
	return  "\\begin{" + name + "}\n" +
				content.toString(i+1) + 
				std::string(i, ' ') + "\\end{" + name + "}";
}

std::string CurlyBracketEnvironment::toString(int i) const {
	return "{\n" + content.toString(i+1) + std::string(i, ' ') + "}";
}

DefaultCommand::DefaultCommand() {}
DefaultCommand::DefaultCommand(std::string name, Contents content): name(name), content(content) {}
std::string DefaultCommand::toString(int i) const {
	std::string result = "\\" + name;
	if(!content.empty()) result+= "{\n" + content.toString(i+1) + std::string(i, ' ') + "}";
	return result;
}

SimpleDefaultCommand::SimpleDefaultCommand() {}
SimpleDefaultCommand::SimpleDefaultCommand(std::string name, std::string content): name(name), content(content) {}
std::string SimpleDefaultCommand::toString(int i) const {
	return "\\" + name + "{" + content + "}";
}

template <class T>
Content::Content(T content): content(content) {}
Content::Content() {}
void Content::operator=(const Content& temp) {
	content = temp.content;
}
std::string Content::toString(int i) const {
	return std::visit([&i](auto temp) ->std::string {return temp.toString(i);}, content);
}

std::string Contents::toString(int i) const {
	std::string result;
	//i+=1;
	for(const Content& temp: *this) {
		result+= std::string(i, ' ') + temp.toString(i) + '\n';
	}
	return result;	
}


Content Parse(TKNZR::Tokenizer& stream) {
	TKNZR::Content current;
	//DefaultEnvironmentHandler environmentHandler;
	EnvironmentHandler* environmentHandler = new DefaultEnvironmentHandler();
	stream >> current;
	environmentHandler->activate(Content(current));
	while(stream >> current) {
		while(environmentHandler->handle(Content(current)) == HandlerResult::NOT_FOR_ME);
	}
	Content temp = environmentHandler->end();
	delete environmentHandler;
	return temp;
}


}
}