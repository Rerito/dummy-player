
#include "gtest/gtest.h"
#include "model/music_cache.hpp"
#include "model/access.hpp"
#include "model/set_track.hpp"
#include "model/add_track.hpp"
#include "model/track_navigation.hpp"
#include "model/playlist_shuffle.hpp"

using music_cache_t = dp::music_cache<int, std::string, dp::playlist_shuffler<> >;

struct MusicCacheTest : testing::Test {
    music_cache_t mcache_;

    virtual void SetUp() override {
        dp::add_track(mcache_, 0, "Hold the line");
        dp::add_track(mcache_, 1, "Holy wars... The punishment due");
        dp::add_track(mcache_, 2, "Act like you know");
    }

    virtual void TearDown() override {
        mcache_ = music_cache_t {};
    }
};

TEST_F(MusicCacheTest, SetTrack) {
    ASSERT_EQ(dp::access::get_playlist(mcache_).size(), 3u);
    dp::set_track(mcache_, 0);
    auto cur_track = mcache_.get_current_track();
    ASSERT_TRUE(cur_track) << "Expected current track to be set";
    ASSERT_EQ(cur_track->get(), std::string("Hold the line"));
}

TEST_F(MusicCacheTest, NextTrack) {
    dp::set_track(mcache_, 1);
    dp::next_track(dp::RepeatMode::NO_REPEAT, mcache_);
    auto cur_track = mcache_.get_current_track();
    ASSERT_TRUE(cur_track);
    ASSERT_EQ(cur_track->get(), std::string("Act like you know"));
}
