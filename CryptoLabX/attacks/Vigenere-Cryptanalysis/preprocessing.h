cat > include/preprocessing.h <<'EOF'
#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <string>

std::string clean_ciphertext(const std::string& ciphertext);

#endif
EOF
