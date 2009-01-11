namespace vapor {
	namespace vfs {

long getFileSize(FILE *fp)
{
	if(!fp) return -1;

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return fileSize;
}

} } // end namespaces
