#include "Scope.h"

Scope::Scope(std::string identifier) {
	this->identifier = identifier;
}

std::string Scope::get_identifier() {
	return identifier;
}