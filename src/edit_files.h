#ifndef EDIT_FILES_H
#define EDIT_FILES_H

//------------------[referenced from edit_files.cpp]-----------------//
EditView *CreateEditView(const char *filename);
static EditView *InitEVFromFile(const char *fname);
static EditView *InitAsNewDocument();
EditView *DoFileOpen(const char *filename);
EditView *DoFileOpenAtLine(const char *filename, int lineNo, int x_start, int x_end);
EditView *FindEVByDocID(uint id);
EditView *FindEVByFilename(const char *filename);

#endif // EDIT_FILES_H
