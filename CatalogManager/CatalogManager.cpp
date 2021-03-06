
#include "CatalogManager.h"
#include "../BufferManager/BufferManager.h"
#include "../Interpreter/Lex/Analysis.hpp"

extern MBuffer m1;
extern MBuffer mybuffer;

bool Create_table(Table table)
{
	ofstream out;
	int i;
	bool exist = Judge_table_exist(table.table_name);

	if(exist)
	{
		return false;
	} 

	out.open("Tablelist.txt",ios::app);

	out<<table.table_name<<endl;
	out.close();
	
	string table_info = table.table_name + "_table.info";
	out.open(table_info.c_str(),ios::app);
	for(i=0;i<table.attr_count;i++)
	{
		out<<table.attrs[i].attr_name<<" ";
		out<<table.attrs[i].attr_key_type<<" ";
		out<<table.attrs[i].attr_type<<" ";
		out<<table.attrs[i].attr_len<<" ";
		out<<table.attrs[i].attr_id<<endl;
	}
	out.close();
	
	string table_rec = table.table_name + "_table.rec";
	out.open(table_rec.c_str(),ios::app);
	out.close();
	return true;
}

bool Create_index(Index index)
{

	bool exist = Judge_index_exist(index.index_name);
	
	if(exist)
	{
		printf("error: Index '%s' exists\n", index.index_name.c_str());
		return false;
	}
	exist = Judge_table_exist(index.table_name);
	if(!exist)
	{
		printf("error: Table '%s' doesn't exist\n", index.table_name.c_str());
		return false;
	}
	exist = Judge_attr_exist(index.table_name, index.attr_name);
	if(!exist)
	{
        printf("error: attribute '%s' does not exists in the table", index.attr_name.c_str());
		return false;
	}
	ofstream out;
	out.open("Indexlist.txt",ios::app);
	out<<index.index_name<<" "<<index.table_name<<" "<<index.attr_name<<endl;
	out.close();
	
	string index_rec = index.index_name + "_index.rec";
	out.open(index_rec.c_str(),ios::app);
	out.close();
    return true;
}

bool Drop_table(string table_name)
{
	bool exist = Judge_table_exist(table_name);
	if(!exist)
	{
		return false;
	}
	string table_rec = table_name + "_table.rec";
	remove(table_rec.c_str()); 
	string table_info = table_name + "_table.info";
	remove(table_info.c_str());
	
    m1.clearBuffer();
    mybuffer.clearBuffer();
    
	string table_list = "";
	char Table_name[32];
	string tablename;
	ifstream in;
	in.open("Tablelist.txt",ios::in);
	while(!in.eof())
	{
		in.getline(Table_name,32);
		tablename = Table_name;
		if(tablename == "" && in.eof())
		{
			break;
		}
		if(tablename != "" && tablename != table_name)
		{
			table_list += tablename + '\n'; 
		}
	}
	in.close();
	
	remove("Tablelist.txt");
	ofstream out;
	out.open("Tablelist.txt", ios::app);
	out<<table_list;
	out.close();
	return true; 
}

bool Drop_index(string index_name)
{
	bool exist = Judge_index_exist(index_name);
	if(!exist)
	{
		return false;
	}
	string index_rec = index_name + "_index.rec";
	remove(index_rec.c_str());
    
    //-----------------------------
    m1.clearBuffer();
    mybuffer.clearBuffer();
	
	string index_list = "";
	char Index_name[128];
	string indexname;
	ifstream in;
	in.open("Indexlist.txt",ios::in);
	int i;
	while(!in.eof())
	{
		in.getline(Index_name,128);
		indexname = Index_name;
		if(indexname!="")
		{
			i=0;
			while(indexname.at(i)!=' ')
			{
				i++;
			}
			indexname = indexname.substr(0,i);
		}
		if(indexname == "" && in.eof())
		{
			break;
		}
		if(indexname != "" && indexname != index_name)
		{
			indexname = Index_name;
			index_list += indexname + '\n'; 
		}
	}
	in.close();
	
	remove("Indexlist.txt");
	ofstream out;
	out.open("Indexlist.txt",ios::app);
	out<<index_list;
	out.close();
	return true; 
}

bool Judge_attr_primary_unique(string table_name, string attr_name)
{
	fstream in;
	int i;
	string table_info = table_name + "_table.info";
	in.open(table_info.c_str(),ios::in);
	char attr[128];
	string attrname;
	char attr_key = '0';
	while(!in.eof())
	{
		in.getline(attr,128);
		attrname = attr;
		if(attrname!="")
		{
			i=0;
			while(attrname.at(i)!=' ')
			{
				i++;
			}
			attrname = attrname.substr(0,i);
		}
		if(attrname == attr_name)
		{
			attr_key = attr[i+1];
			break; 
		}
	}
	in.close();
	if(attr_key!='0')
		return true;
	else
		return false;
}

bool Judge_table_exist(string table_name)
{
	ifstream in;
	bool flag = false;
	char Table_name[32];
	string tablename;

	in.open("Tablelist.txt", ios::in);
    
    if (!in) {
        ofstream out;
        out.open("Tablelist.txt", ios::out);
        return false;
    }

	while(!in.eof())
	{
		in.getline(Table_name,32);

		tablename = Table_name;

		if(tablename == table_name)
		{
			flag = true; 
			break;
		}
	}
	in.close();
	return flag; 
}

bool Judge_index_exist(string index_name)
{
	ifstream in;
	int i;
	bool flag = false;
	char index[128];
	string indexname;
	in.open("Indexlist.txt",ios::in);
    
    if (!in) {
        ofstream out;
        out.open("Indexlist.txt", ios::out);
        return false;
    }
    
	while(!in.eof())
	{
		in.getline(index,128);
		indexname = index;
		if(indexname!="")
		{
			i=0;
			while(indexname.at(i)!=' ')
			{
				i++;
			}
			indexname = indexname.substr(0,i);
		}
		if(indexname == index_name)
		{
			flag = true;
			break; 
		}
	}
	in.close();
	return flag; 
}

bool Judge_attr_exist(string table_name,string attr_name)
{
	ifstream in;
	bool flag = false;
	int i;
	string table_info = table_name + "_table.info";
	
	in.open(table_info.c_str(),ios::in);

	char attr[128];
	string attrname;
	
	while(!in.eof())
	{
		in.getline(attr,128);
		attrname = attr;
		if(attrname!="")
		{
			i=0;
			while(attrname.at(i)!=' ')
			{
				i++;
			}
			attrname = attrname.substr(0,i);
		}
		if(attrname == attr_name)
		{
			flag = true;
			break; 
		}
	}
	in.close();
	return flag;
}


Table Read_Table_Info(string table_name)
{
	Table table;
	table.table_name = table_name;
	table.attr_count = 0;
	string file_name = table_name + "_table.info";
	ifstream in;
	in.open(file_name.c_str(),ios::in);
    
	if(!in)
	{
		return table;
	}

	string info;
	int k=0;

	while(!in.eof())
	{
		in >> table.attrs[k].attr_name;		
		in >> table.attrs[k].attr_key_type;
		in >> table.attrs[k].attr_type;
		in >> table.attrs[k].attr_len;
		in >> table.attrs[k].attr_id;
		
		k++; 		
	}
	table.attr_count = k-1;
	in.close();
	return table;	
}


string Find_index_name(string table_name,string attr_name)
{
	ifstream in;
	int i,j,k;

	char Index[128];
	string index;
	string indexname = "";
	string tablename;
	string attrname;
	in.open("Indexlist.txt",ios::in);
	if(!in)
	{
		cout<<"No such info file!"<<endl;
		return "";
	}
	while(!in.eof())
	{	
		in.getline(Index,128);
		index = Index;
		if(index!="")
		{		
			index += '\n';
			i=0;
			while(index.at(i)!=' ')
			{
				i++;
			}
			indexname = index.substr(0,i);
			j=i+1;
			while(index.at(j)!=' ')
			{
				j++;
			}
			tablename = index.substr(i+1,j-i-1);
			k=j+1;
			while(index.at(k)!='\n')
			{
				k++;
			}
			attrname = index.substr(j+1,k-j-1);

			if(tablename == table_name && attrname == attr_name)
				break;
            indexname = "";
		}
	}
	in.close();
	return indexname; 
}






