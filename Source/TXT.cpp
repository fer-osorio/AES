#include"TXT.hpp"
#include<fstream>

TXT::TXT(const char* fname): name(fname) { // -Building from file.
    std::ifstream file;
    file.open(fname);
    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        this->size = fileSize;
        file.seekg(0, std::ios::beg);
        this->content = new char[fileSize];
        file.read(this->content, fileSize);
        file.close();
    } else {
        char errmsg[] = "\nIn TXT.cpp file, TXT::TXT(const char* fname): "
                        "Could not open file ";
        std::cout << errmsg << fname << '\n';
        throw errmsg;
    }
    std::cout << "\nTXT.cpp; line 21; name:" << this->name.getNameString() << "\n";
}

TXT::TXT(FileName& fname): name(fname) {
    std::ifstream file;
    file.open(fname.getNameString());
    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        this->size = fileSize;
        file.seekg(0, std::ios::beg);
        this->content = new char[fileSize];
        file.read(this->content, fileSize);
        file.close();
    } else {
        throw "\nCould not open file...\n";
    }
}

TXT::TXT(const TXT& t): name(t.name), size(t.size) {
    this->content = new char[t.size];
    for(unsigned i = 0; i < t.size; i++) this->content[i] = t.content[i];
}

void TXT::save(const char* fname) {
    std::cout << "From TXT.cpp; line 51; name: " << this->name.getNameString() << '\n';
    std::ofstream file;
    if(fname == NULL) fname = this->name.getNameString();
    std::cout << "From TXT.cpp; line 54; name: " << fname << '\n';
    file.open(fname);
    if(file.is_open()) {
        file.write(this->content, this->size);
        file.close();
    } else {
        throw "File could not be written.";
    }
    std::cout << "From TXT.cpp; line 61; name: " << this->name.getNameString() << '\n';
}

TXT& TXT::operator = (const TXT& t) {
    if(this != &t) {
        this->name = t.name;
        this->size = t.size;
        if(content != NULL) delete[] content;
        this->content = new char[t.size];
        for(unsigned i = 0; i < t.size; i++) this->content[i] = t.content[i];
    }
    return *this;
}

void encryptCBC(TXT& txt, const AES& e) {
    std::cout << "\n TXT.cpp; line 77; name; " << txt.name.getNameString() << '\n';
    e.encryptCBC(txt.content, txt.size);
    txt.save();
}

void decryptCBC(TXT& txt, const AES& e) {
    e.decryptCBC(txt.content, txt.size);
    txt.save();
}

void encryptECB(TXT& txt, const AES& e) {
    e.encryptECB(txt.content, txt.size);
    txt.save();
}

void decryptECB(TXT& txt, const AES& e) {
    e.decryptECB(txt.content, txt.size);
    txt.save();
}
