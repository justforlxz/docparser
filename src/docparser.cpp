// SPDX-FileCopyrightText: 2021 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "docparser.h"

#include "fileext/doc/doc.hpp"
#include "fileext/docx/docx.hpp"
#include "fileext/excel/excel.hpp"
#include "fileext/odf/odf.hpp"
#include "fileext/pdf/pdf.hpp"
#include "fileext/ppt/ppt.hpp"
#include "fileext/pptx/pptx.hpp"
#include "fileext/rtf/rtf.hpp"
#include "fileext/txt/txt.hpp"
#include "fileext/xlsb/xlsb.h"

#include <memory>
#include <iostream>

std::string DocParser::convertFile(const std::string &filename)
{
    std::string content = "";
    std::string suffix = filename.substr(filename.find_last_of('.') + 1);
    if (suffix.empty())
        return content;

    std::unique_ptr<fileext::FileExtension> document;
    try {
        // 比较后缀名，不区分大小写
        if (!strcasecmp(suffix.c_str(), "docx")) {
            document.reset(new docx::Docx(filename));
        } else if (!strcasecmp(suffix.c_str(), "pptx") || !strcasecmp(suffix.c_str(), "ppsx")) {
            document.reset(new pptx::Pptx(filename));
        } else if (!strcasecmp(suffix.c_str(), "txt") || !strcasecmp(suffix.c_str(), "text") ||
                   !strcasecmp(suffix.c_str(), "md") || !strcasecmp(suffix.c_str(), "markdown")) {
            document.reset(new txt::Txt(filename));
        } else if (!strcasecmp(suffix.c_str(), "doc") || !strcasecmp(suffix.c_str(), "dot") ||
                   !strcasecmp(suffix.c_str(), "wps")) {
            document.reset(new doc::Doc(filename));
        } else if (!strcasecmp(suffix.c_str(), "rtf")) {
            document.reset(new rtf::Rtf(filename));
        } else if (!strcasecmp(suffix.c_str(), "odg") || !strcasecmp(suffix.c_str(), "odt") ||
                   !strcasecmp(suffix.c_str(), "ods") || !strcasecmp(suffix.c_str(), "odp")) {
            document.reset(new odf::Odf(filename));
        } else if (!strcasecmp(suffix.c_str(), "xls") || !strcasecmp(suffix.c_str(), "xlsx")) {
            document.reset(new excel::Excel(filename, suffix));
        } else if (!strcasecmp(suffix.c_str(), "xlsb")) {
            document.reset(new xlsb::Xlsb(filename));
        } else if (!strcasecmp(suffix.c_str(), "ppt") || !strcasecmp(suffix.c_str(), "pps") ||
                   !strcasecmp(suffix.c_str(), "dps")) {
            document.reset(new ppt::Ppt(filename));
        } else if (!strcasecmp(suffix.c_str(), "pdf")) {
            document.reset(new pdf::Pdf(filename));
        } else {
            throw std::logic_error("Unsupported file extension: " + filename);
        }

        document->convert();
        content = document->m_text;
    } catch(const std::logic_error &error) {
        std::cout << error.what() << std::endl;
    } catch (...) {
        std::cerr << "Parse failed: " << filename << std::endl;
    }

    document.reset();
    return content;
}
