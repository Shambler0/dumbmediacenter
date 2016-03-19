#define LF "\n"

#define safestrbytes(dst) ( sizeof(dst) + ( sizeof(dst[0])*0 ) ) //error if dst is a pointer

#define safestrclr(dst) memset(dst, 0, safestrbytes(dst))
#define safestrcpy(dst, src) snprintf(dst, safestrbytes(dst), "%s", src)
#define safefgets(dst, f) dst[0]='\0'; fgets(dst, safestrbytes(dst), f);
#define safefgetsnolf(dst, f) safefgets(dst, f); if (strlen(dst)>strlen(LF)) {dst[strlen(dst)-strlen(LF)]='\0';} else {dst[0]='\0';}

#define safesprintf1(dst, fmt, s1) snprintf(dst, safestrbytes(dst), fmt, s1)
#define safesprintf2(dst, fmt, s1, s2) snprintf(dst, safestrbytes(dst), fmt, s1, s2)
#define safesprintf3(dst, fmt, s1, s2, s3) snprintf(dst, safestrbytes(dst), fmt, s1, s2, s3)
#define safesprintf4(dst, fmt, s1, s2, s3, s4) snprintf(dst, safestrbytes(dst), fmt, s1, s2, s3, s4)
#define safesprintf5(dst, fmt, s1, s2, s3, s4, s5) snprintf(dst, safestrbytes(dst), fmt, s1, s2, s3, s4, s5)
#define safesprintf6(dst, fmt, s1, s2, s3, s4, s5, s6) snprintf(dst, safestrbytes(dst), fmt, s1, s2, s3, s4, s5, s6)

#define ddd printf

