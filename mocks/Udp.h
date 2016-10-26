class UDP {
public:
  UDP();
  ~UDP();
  int length();
  void write(unsigned char *buffer, int length);
  unsigned char *buffer;
  int bufferLength;
};