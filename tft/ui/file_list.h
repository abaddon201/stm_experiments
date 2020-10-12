/*
 * file_list.h
 *
 *  Created on: 26 сент. 2020 г.
 *      Author: Abby
 */

#ifndef FILE_LIST_H_
#define FILE_LIST_H_

#include "ui_element.h"

class FileList: public UIElement {
public:

    void setDir(const char *dir);
    void show() override;
};

#endif /* FILE_LIST_H_ */
