#include "parser.hpp"

int main()
{
    std::ifstream stream("./file.dcm");

    Debug(dicom::file::is_dicom(stream));

    std::vector<dicom::file::AbstractTag*> dataset;

    dicom::file::read(stream, dataset, utility::Endian::Litle, true);


    std::cout << "tags: " << dataset.size() << std::endl;

    return 0;
}
