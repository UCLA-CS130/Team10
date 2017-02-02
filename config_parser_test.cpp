#include "gtest/gtest.h"
#include "config_parser.hpp"
#include <sstream>
#include <string>

class NginxStringConfigTest : public ::testing::Test {
protected:
  bool ParseString(const std::string config_string) {
     std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
}; //Fixture

TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size()) << "Config has one statements";
  EXPECT_EQ(out_config_.statements_[0]->tokens_[0], "foo");
  EXPECT_EQ(out_config_.statements_[0]->tokens_[1], "bar");
}

TEST_F(NginxStringConfigTest, Comment) {
  EXPECT_TRUE(ParseString("#comment\nfoo bar;"));
  EXPECT_TRUE(ParseString("#comment\r\nfoo bar;"));
  EXPECT_TRUE(ParseString("foo bar; #comment"));
  EXPECT_TRUE(ParseString("foo bar;\n #comment\n"));
  EXPECT_EQ(4, out_config_.statements_.size());
  EXPECT_EQ(out_config_.statements_[0]->tokens_[0], "foo");
  EXPECT_EQ(out_config_.statements_[1]->tokens_[1], "bar");
}

TEST_F(NginxStringConfigTest, EmptyInput) {
  EXPECT_TRUE(ParseString(""));
  EXPECT_TRUE(ParseString("   "));
  EXPECT_TRUE(ParseString("\t"));
  EXPECT_TRUE(ParseString("\t\n"));
}

TEST_F(NginxStringConfigTest, InvalidConfig) {
  EXPECT_FALSE(ParseString("{ foo bar;"));
  EXPECT_FALSE(ParseString("foo bar;}"));
  EXPECT_FALSE(ParseString("foo bar;{"));
  EXPECT_FALSE(ParseString("} foo bar;"));
  EXPECT_FALSE(ParseString("foo { bar; baz}}"));
  EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(ParseString("server { listen 80;}"));
}

TEST_F(NginxStringConfigTest, InvalidSemicolons) {
  EXPECT_FALSE(ParseString("foo { bar; };  "));
  EXPECT_FALSE(ParseString("foo { bar; } \n\"baz\""));
}

TEST_F(NginxStringConfigTest, MismatchedQuotes) {
  EXPECT_FALSE(ParseString("\"foo; "));
  EXPECT_FALSE(ParseString("\"foo;\";\" "));
}