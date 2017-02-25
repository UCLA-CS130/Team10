#include "gtest/gtest.h"
#include "../mime_types.hpp"
#include <string>

TEST(Extension_to_type_test, SimpleTest) {
  EXPECT_EQ("image/gif", mime_types::extension_to_type("gif"));
  EXPECT_EQ("text/html", mime_types::extension_to_type("htm"));
  EXPECT_EQ("text/html", mime_types::extension_to_type("html"));
  EXPECT_EQ("image/jpeg", mime_types::extension_to_type("jpg"));
  EXPECT_EQ("image/png", mime_types::extension_to_type("png"));
  EXPECT_EQ("text/plain", mime_types::extension_to_type("pdf"));
}
