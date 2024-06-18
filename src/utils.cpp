#include <ft_irc.hpp>

// TODO: fix this
std::string readfd(int fd) {
  std::string result;
  char buff[100];
  int bytes = read(fd, buff, 100);
  while (bytes) {
    buff[bytes] = '\0';
    std::cout << "bytes " << bytes << " buff " << buff << std::endl;
    if (bytes < 100) {
      result.append(buff);
      bytes = 0;
    } else {
      bytes = read(fd, buff, 100);
    }
  }
  return result;
}

unsigned int stringAddressToBytes(std::string str) {
  int a, b, c, d;
  char arr[4];
  sscanf(str.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d);
  arr[0] = a;
  arr[1] = b;
  arr[2] = c;
  arr[3] = d;
  return *(unsigned int *)arr;
}

std::string getAddressFromSockAddrin(const struct sockaddr_in addr) {
  char host_buffer[NI_MAXHOST];

  if (int err = getnameinfo(reinterpret_cast<const struct sockaddr *>(&addr),
                            sizeof(addr), host_buffer, sizeof(host_buffer),
                            NULL, 0, NI_NAMEREQD)) {
    return "Not known";
  }
  return std::string(host_buffer);
}

std::ostream &operator<<(std::ostream &os, const Socket<sockaddr_in> &value) {
  os << "Socket In {\n"
     << "\tAddress fd:           " << value.getFd() << "\n";
  try {
    sockaddr_in addr = value.getRawAddr();
    os << "\tPort:                 " << ntohs(addr.sin_port) << "\n";
  } catch (const std::exception &e) {
    os << "\tPort:                 " << "unavailable\n";
  }
  try {
    sockaddr_in addr = value.getRawAddr();
    os << "\tAddress:              " << getAddressFromSockAddrin(addr) << "\n";
  } catch (const std::exception &e) {
    os << "\tAddress:              " << "unavailable\n";
  }
  os << "\tIs Writable:          " << value.isWritable() << "\n"
     << "\tLast Pending Message: " << value.getPendingMessagesToWrite().front()
     << "\n}";

  return os;
}

std::ostream &operator<<(std::ostream &os, const User &value) {
  os << "User {\n"
     << "\tUsername: " << value.username << "\n"
     << "\tNickname: " << value.nickname << "\n"
     << "}\n";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Client<sockaddr_in> &value) {
  os << "Client {\n"
     << "\t" << value.user << "\n"
     << "\t" << value.socket << "\n"
     << "}\n";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Channel<sockaddr_in> &value) {
  std::map<int, Client<sockaddr_in> >::iterator it = value.getClients().begin();

  os << "Channel {\n"
     << "\tTopic: " << value.getTopic() << "\n"
     << "\tClients: ";

  for (; it != value.getClients().end(); it++) {
    os << it->second << ", ";
  }
  os << "\t]\n";
  os << "}\n";
  return os;
}

std::string readFile(std::string filename, char separator) {
  std::ifstream file(filename.c_str());

  if (file.fail()) {
    std::string err = "Error opening the file: ";
    err.append(filename);
    file.close();
    throw std::runtime_error(err);
  }

  DIR *dir = opendir(filename.c_str());

  if (dir != NULL) {

    std::string err = "Error opening the file: ";
    err.append(filename);
    closedir(dir);
    throw std::runtime_error(err);
  }

  std::string line, result;
  unsigned int i = 0;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    if (i != 0) {
      result.append("\n");
    }
    result.append(line);
    i++;
  }

  file.close();
  return result;
}

std::string replaceAll(std::string str, const std::string &from,
                       const std::string &to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length(); // Handles case where 'to' is a substring of 'from'
  }
  return str;
}

std::vector<std::string> split(std::string str) {
  std::vector<std::string> splitted;
  std::string del = " ";
  size_t pos = str.find(del);

  std::string token;
  while (pos != std::string::npos) {
    token = str.substr(0, pos);
    splitted.push_back(token);
    str.erase(0, pos + del.length());
    pos = str.find(del);
  }
  splitted.push_back(str);
  return splitted;
}

std::vector<std::string> splitByComma(const std::string &data) {
  std::string token;
  std::vector<std::string> tokens;
  std::istringstream iss(data);

  while (std::getline(iss, token, ',')) {
    tokens.push_back(token);
  }
  return (tokens);
}
