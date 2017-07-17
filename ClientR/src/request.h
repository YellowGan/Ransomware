#ifndef REQUEST_H
#define REQUEST_H

#include "network.h"

std::string createId(std::string password);

void payment(std::string IdSession);

std::string checkPayment (std::string IdSession);

void deleteData (std::string IdSession);

#endif /* REQUEST_H */