#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <ctime>
#include <fstream>
enum class state { inputStatcSizeBlock,inputDynamicSizeBlock, outputBlock, terminate };

const std::string startDynamicBlock = std::string({ "{" });
const std::string endDynamicBlock = std::string({ "}" });
std::queue<state> stateQueue;
state currentState;

auto& inputNewCommand(std::string &command) {
	 auto& a = std::getline(std::cin, command);
	 if (!a){
		if(currentState==state::inputStatcSizeBlock) {
			stateQueue.push(state::outputBlock);
			stateQueue.push(state::terminate);
		}
		stateQueue.push(state::terminate);	
	}
	 return a;
}

void inputSaticSizeBlock(int const & countOfCommandsInBlock,std::vector<std::string> &commandsInBlock) {
	std::string currentCommand;
	commandsInBlock.reserve(countOfCommandsInBlock);
	for (int i = 0; i < countOfCommandsInBlock&&inputNewCommand(currentCommand); i++) {
		if (i == 0) {
			auto firstCommandTime = std::time(NULL);
			commandsInBlock.emplace_back(std::to_string(firstCommandTime));
		}
		if (currentCommand == startDynamicBlock) {
			stateQueue.push(state::outputBlock);
			stateQueue.push(state::inputDynamicSizeBlock);
			return;
		}
		commandsInBlock.emplace_back(currentCommand);
	}
	stateQueue.push(state::outputBlock);
	stateQueue.push(state::inputStatcSizeBlock);
}
void outputBlock(std::vector<std::string> const &commandsInBlock) {
	if (commandsInBlock.size()>1) {
		
		std::ofstream outputFile(std::string("bulk") + commandsInBlock[0]);
		std::cout << "bulk: ";
		outputFile << "bulk ";
		for (int i = 1; i < commandsInBlock.size(); i++) {
			std::cout << commandsInBlock[i];
			outputFile << commandsInBlock[i];
			if (i != commandsInBlock.size() - 1) {
				std::cout << ", ";
				outputFile << ", ";
			}
		}
		outputFile << std::endl;
		std::cout << std::endl;
	}
}
void inputDynamicSizeBlock(std::vector<std::string> &commandsInBlock) {
	int endOfDynamicBlockCounter = 1;
	std::string currentComand;
	bool firstLineRecived = false;
	while (endOfDynamicBlockCounter&&inputNewCommand(currentComand))
	{
		if (!firstLineRecived) {
			auto firstCommandTime = std::time(NULL);
			commandsInBlock.emplace_back(std::to_string(firstCommandTime));
			firstLineRecived = true;
		}
		if (currentComand == startDynamicBlock) {
			endOfDynamicBlockCounter++;
		}
		else if (currentComand == endDynamicBlock) {
			endOfDynamicBlockCounter--;
		}
		else {
			commandsInBlock.emplace_back(currentComand);
		}
	}
	if (endOfDynamicBlockCounter==0) {
		stateQueue.push(state::outputBlock);
		stateQueue.push(state::inputStatcSizeBlock);
	}
}
void processCommands(int const& countOfCommandsInBlock) {
	stateQueue.push(state::inputStatcSizeBlock);
	std::vector<std::string> commandsInBlock;
	while (!stateQueue.empty()){
		currentState = stateQueue.front();
		stateQueue.pop();
		switch (currentState)
		{
		case state::inputStatcSizeBlock:
			inputSaticSizeBlock(countOfCommandsInBlock, commandsInBlock);
			break;
		case state::inputDynamicSizeBlock:
			inputDynamicSizeBlock(commandsInBlock);
			break;
		case state::outputBlock:
			outputBlock(commandsInBlock);
			commandsInBlock.clear();
			break;
		case state::terminate:
			return;
			break;
		default:
			break;
		}
	}

}

int main(int argc,char **argv)
{
	if (argc > 1) {
		int countOfCommandsInBlock = atoi(argv[1]);
		processCommands(countOfCommandsInBlock);
	}
	else {
		std::cout << "Please transmit count of commands in block";
		return 0;
	}
	return 0;
}

