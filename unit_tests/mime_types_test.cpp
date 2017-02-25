#include "gtest/gtest.h"
#include "../mime_types.hpp"
#include <string>

TEST(Extension_to_type_test, SimpleTest) {
  EXPECT_EQ("image/gif", mime_types::extension_to_type("gif"));
  /*EXPECT_EQ("text/htm", extension_to_type("htm"));
  EXPECT_EQ("text/html", extension_to_type("html"));
  EXPECT_EQ("image/jpeg", extension_to_type("jpg"));
  EXEPCT_EQ("image/png", extension_to_type("png"));
  EXPECT_EQ("text/plain", extension_to_type("pdf"));*/
}
