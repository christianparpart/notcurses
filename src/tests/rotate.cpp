#include "main.h"
#include <cmath>
#include <vector>

TEST_CASE("Rotate") {
  auto nc_ = testing_notcurses();
  if(!nc_){
    return;
  }
  if(!notcurses_canutf8(nc_)){
    CHECK(0 == notcurses_stop(nc_));
    return;
  }
  int dimy, dimx;
  struct ncplane* n_ = notcurses_stddim_yx(nc_, &dimy, &dimx);
  REQUIRE(n_);

  uint64_t ul, ur, ll, lr;
  ul = ur = ll = lr = 0;
  ncchannels_set_fg_rgb(&ul, 0x40f040);
  ncchannels_set_bg_rgb(&ul, 0x40f040);
  ncchannels_set_fg_rgb(&ll, 0xf040f0);
  ncchannels_set_bg_rgb(&ll, 0xf040f0);
  ncchannels_set_fg_rgb(&ur, 0x40f040);
  ncchannels_set_bg_rgb(&ur, 0x40f040);
  ncchannels_set_fg_rgb(&lr, 0xf040f0);
  ncchannels_set_bg_rgb(&lr, 0xf040f0);

  // use half of each dimension
  SUBCASE("RotateRGBACW") {
    int height = dimy / 2;
    int width = dimx / 2;
    std::vector<uint32_t> rgba(width * height, htole(0xffbbccff));
    for(int i = 0 ; i < height * width / 2 ; ++i){
      CHECK(htole(0xffbbccff) == rgba[i]);
    }
    auto ncv = ncvisual_from_rgba(rgba.data(), height, width * 4, width);
    REQUIRE(ncv);
    ncvisual_options opts{};
    auto rendered = ncvisual_render(nc_, ncv, &opts);
    REQUIRE(rendered);
    int pxdimy, pxdimx;
    uint32_t* rgbaret = ncplane_as_rgba(rendered, NCBLIT_2x1,
                                        0, 0, -1, -1, &pxdimy, &pxdimx);
    REQUIRE(rgbaret);
    if(height % 2){
      ++height;
    }
    CHECK(pxdimx == width);
    CHECK(pxdimy == height);
    for(int i = 0 ; i < height * width / 2 ; ++i){
      if(rgbaret[i] & NC_BG_RGB_MASK){
        CHECK(htole(rgbaret[i]) == htole(rgba[i]));
      }
    }
    free(rgbaret);
    CHECK(0 == notcurses_render(nc_));
    for(int x = 0 ; x < width ; ++x){
      uint16_t stylemask;
      uint64_t channels;
      char* c = notcurses_at_yx(nc_, 0, x, &stylemask, &channels);
      REQUIRE(c);
      CHECK(0 == strcmp(c, " "));
      if(ncchannels_fg_rgb(channels) & NC_BG_RGB_MASK){
        CHECK(0xffccbb == ncchannels_fg_rgb(channels));
      }
      if(ncchannels_bg_rgb(channels) & NC_BG_RGB_MASK){
        CHECK(0xffccbb == ncchannels_bg_rgb(channels));
      }
      free(c);
    }
    if(notcurses_canopen_images(nc_)){
      opts.n = rendered;
      // FIXME check pixels after all rotations
      CHECK(0 == ncvisual_rotate(ncv, M_PI / 2));
      CHECK(ncvisual_render(nc_, ncv, &opts));
      CHECK(0 == notcurses_render(nc_));
      CHECK(0 == ncvisual_rotate(ncv, M_PI / 2));
      CHECK(ncvisual_render(nc_, ncv, &opts));
      CHECK(0 == notcurses_render(nc_));
      CHECK(0 == ncvisual_rotate(ncv, M_PI / 2));
      CHECK(ncvisual_render(nc_, ncv, &opts));
      CHECK(0 == notcurses_render(nc_));
      CHECK(0 == ncvisual_rotate(ncv, M_PI / 2));
      CHECK(ncvisual_render(nc_, ncv, &opts));
      CHECK(0 == notcurses_render(nc_));
    }
    ncvisual_destroy(ncv);
    ncplane_destroy(rendered);
    CHECK(0 == notcurses_render(nc_));
  }

  SUBCASE("RotateRGBACCW") {
    int height = dimy / 2;
    int width = dimx / 2;
    std::vector<uint32_t> rgba(width * height, htole(0xffbbccff));
    auto ncv = ncvisual_from_rgba(rgba.data(), height, width * 4, width);
    REQUIRE(ncv);
    ncvisual_options opts{};
    auto rendered = ncvisual_render(nc_, ncv, &opts);
    REQUIRE(rendered);
    int pxdimy, pxdimx;
    uint32_t* rgbaret = ncplane_as_rgba(rendered, NCBLIT_2x1,
                                        0, 0, -1, -1, &pxdimy, &pxdimx);
    REQUIRE(rgbaret);
    if(height % 2){
      ++height;
    }
    CHECK(pxdimy == height);
    CHECK(pxdimx == width);
    for(int i = 0 ; i < height * width / 2 ; ++i){
      if(rgbaret[i] & NC_BG_RGB_MASK){
        CHECK(htole(rgbaret[i]) == htole(rgba[i]));
      }
    }
    free(rgbaret);
    CHECK(0 == notcurses_render(nc_));
    for(int x = 0 ; x < width ; ++x){
      uint16_t stylemask;
      uint64_t channels;
      char* c = notcurses_at_yx(nc_, 0, x, &stylemask, &channels);
      REQUIRE(c);
      CHECK(0 == strcmp(c, " "));
      if(ncchannels_fg_rgb(channels) & NC_BG_RGB_MASK){
        CHECK(0xffccbb == ncchannels_fg_rgb(channels));
      }
      if(ncchannels_bg_rgb(channels) & NC_BG_RGB_MASK){
        CHECK(0xffccbb == ncchannels_bg_rgb(channels));
      }
      free(c);
    }
    if(notcurses_canopen_images(nc_)){
      // FIXME check pixels after all rotations
      opts.n = rendered;
      CHECK(0 == ncvisual_rotate(ncv, -M_PI / 2));
      CHECK(ncvisual_render(nc_, ncv, &opts));
      CHECK(0 == notcurses_render(nc_));
      CHECK(0 == ncvisual_rotate(ncv, -M_PI / 2));
      CHECK(ncvisual_render(nc_, ncv, &opts));
      CHECK(0 == notcurses_render(nc_));
      CHECK(0 == ncvisual_rotate(ncv, -M_PI / 2));
      CHECK(ncvisual_render(nc_, ncv, &opts));
      CHECK(0 == notcurses_render(nc_));
      CHECK(0 == ncvisual_rotate(ncv, -M_PI / 2));
      CHECK(ncvisual_render(nc_, ncv, &opts));
      CHECK(0 == notcurses_render(nc_));
    }
    ncvisual_destroy(ncv);
    ncplane_destroy(rendered);
    CHECK(0 == notcurses_render(nc_));
  }

  CHECK(0 == notcurses_stop(nc_));

}
