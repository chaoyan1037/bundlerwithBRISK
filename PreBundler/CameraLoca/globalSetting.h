#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <string>
#if 0
const std::string gl_BundleOutFilePath	= "../../database/bundle/bundle.rd.out";

const std::string gl_ImgPathQuery		= "../../database/images/query/";
const std::string gl_ImgPathDB			= "../../database/images/db/";
const std::string gl_ImgListPathQuery	= "../../database/list/list_query.txt";
const std::string gl_ImgListDB			= "../../database/list/list_db.txt";
#endif

extern std::string gl_BundleOutFilePath;
extern std::string gl_ImgPathQuery;
extern std::string gl_ImgPathDB;
extern std::string gl_ImgListPathQuery;
extern std::string gl_ImgListDB;

#ifndef uint 
typedef unsigned int uint;
#endif

#endif