// // --- format elapsed time as hh:mm:ss ---
// String fmt_hms(uint32_t ms) {
//   uint32_t total_s = ms / 1000;
//   int h = total_s / 3600;
//   int m = (total_s % 3600) / 60;
//   int s = total_s % 60;

//   char buf[16];
//   sprintf(buf, "%02d:%02d:%02d", h, m, s);
//   return String(buf);
// }