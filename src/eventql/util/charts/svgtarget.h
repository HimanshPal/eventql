/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License ("the license") as
 * published by the Free Software Foundation, either version 3 of the License,
 * or any later version.
 *
 * In accordance with Section 7(e) of the license, the licensing of the Program
 * under the license does not imply a trademark license. Therefore any rights,
 * title and interest in our trademarks remain entirely with us.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you develop
 * commercial activities involving this program without disclosing the source
 * code of your own applications
 */

#ifndef _libstx_SVGTARGET_H
#define _libstx_SVGTARGET_H
#include <stdlib.h>
#include <vector>
#include <string>
#include "eventql/util/charts/rendertarget.h"
#include "eventql/util/charts/styles/style_default.h"
#include "eventql/util/io/outputstream.h"

namespace util {
namespace chart {

class SVGTarget : public RenderTarget {
public:

  /**
   * Create a new SVG target.
   *
   * @param output_stream the output stream to write to. does not transfer
   *                      ownership!
   */
  SVGTarget(
      OutputStream* output_stream) :
      viewbox_(true),
      output_(output_stream),
      indent_(0) {}

#define SVG_append(...) { output_->printf(__VA_ARGS__); }

#define SVG_appendLine(...) { \
    for(int __i = 0; __i < indent_ * 2; ++__i) SVG_append(" "); \
    SVG_append(__VA_ARGS__); }

  void beginChart(
      int width,
      int height,
      const std::string& class_name) {
    if (viewbox_) {
      SVG_appendLine(
          "<svg viewBox='0 0 %i %i' class='%s'>\n",
          width,
          height,
          escapeString(class_name).c_str());
    } else {
      SVG_appendLine(
          "<svg width='%i' height='%i' class='%s'>\n",
          width,
          height,
          escapeString(class_name).c_str());
    }

    indent_++;
    SVG_appendLine("<style type='text/css'>\n");
    SVG_appendLine("<![CDATA[%s  ]]>\n", kStyleSheetDefault.c_str());
    SVG_appendLine("</style>\n");
  }

  void finishChart() {
    indent_--;
    SVG_appendLine("</svg>\n");
  }

  void drawRect(
      double x,
      double y,
      double width,
      double height,
      const std::string& color,
      const std::string& class_name,
      const std::string& label,
      const std::string& series) override {
    std::string class_str(class_name);
    class_str += " ";

    std::string style_str;
    if (color.size() > 3 && color.size() < 8 && color[0] == '#') {
      style_str += "fill: " + color + ";";
    } else {
      class_str += color;
    }

    // FIXPAUL escape me
    SVG_appendLine(
        "<rect x='%f' y='%f' width='%f' height='%f' class='%s' "
            "fm:series='%s' fm:label='%s' style='%s'></rect>\n",
        x,
        y,
        width,
        height,
        escapeString(class_str).c_str(),
        escapeString(series).c_str(),
        escapeString(label).c_str(),
        escapeString(style_str).c_str());
  }

  void drawLine(
      double x1,
      double y1,
      double x2,
      double y2,
      const std::string& class_name) override {
    SVG_appendLine(
        "<line x1='%f' y1='%f' x2='%f' y2='%f' class='%s' />\n",
        x1,
        y1,
        x2,
        y2,
        escapeString(class_name).c_str());
  }

  void drawText(
      const std::string& text,
      double x,
      double y,
      const std::string& halign,
      const std::string& valign,
      const std::string& class_name,
      double rotate = 0.0f) override {
    char transform[256];

    if (rotate == 0.0f) {
      *transform = 0;
    } else {
      snprintf(
          transform,
          sizeof(transform),
          "transform='translate(0,0) rotate(%f %f %f)'",
          rotate,
          x,
          y);
    }

    SVG_appendLine(
        "<text x='%f' y='%f' style='text-anchor:%s; dominant-baseline:%s;' "
            "class='%s' %s>%s</text>\n",
        x,
        y,
        halign.c_str(),
        valign.c_str(),
        escapeString(class_name).c_str(),
        transform,
        escapeString(text).c_str());
  }

  void drawPoint(
      double x,
      double y,
      const std::string& point_type,
      double point_size,
      const std::string& color,
      const std::string& class_name /* = "" */,
      const std::string& label /* = "" */,
      const std::string& series /* = "" */) override {
    std::string class_str(class_name);
    class_str += " ";

    std::string style_str;
    if (color.size() > 3 && color.size() < 8 && color[0] == '#') {
      style_str += "fill: " + color + ";";
    } else {
      class_str += color;
    }

    /* point_type: circle */
    // FIXPAUL escape label
    SVG_appendLine(
        "<circle cx='%f' cy='%f' r='%f' class='%s' fm:label='%s' "
            "fm:series='%s', style='%s'></circle>\n",
        x,
        y,
        point_type == "none" ? 0 : point_size,
        escapeString(class_str).c_str(),
        escapeString(label).c_str(),
        escapeString(series).c_str(),
        escapeString(style_str).c_str());
  }

   void drawPath(
      const std::vector<std::pair<double, double>>& points,
      const std::string& line_style,
      double line_width,
      bool smooth,
      const std::string& color,
      const std::string& class_name = "") override {
    std::string class_str(class_name);
    class_str += " ";

    std::string style_str;
    if (color.size() > 3 && color.size() < 8 && color[0] == '#') {
      style_str += "stroke: " + color + ";";
    } else {
      class_str += color;
    }

    SVG_appendLine(
        "<path stroke-width='%f' class='%s' style='%s' d='",
        line_width,
        escapeString(class_str).c_str(),
        escapeString(style_str).c_str());

    for (int i = 0; i < points.size(); ++i) {
      if (i == 0) {
        SVG_append("M%f %f ", points[i].first, points[i].second);
      } else if (smooth) {
        SVG_append("L%f %f ", points[i].first, points[i].second);
      } else {
        SVG_append("L%f %f ", points[i].first, points[i].second);
      }
    }

    SVG_append("' />\n");
  }

  void beginGroup(const std::string& class_name) override {
    SVG_appendLine("<g class='%s'>\n", escapeString(class_name).c_str());
    indent_++;
  }

  void finishGroup() override {
    indent_--;
    SVG_appendLine("</g>\n");
  }

  // FIXPAUL
  std::string escapeString(const std::string& src) {
    return src;
  }

  void setViewbox(bool vb) {
    viewbox_ = vb;
  }

protected:
  bool viewbox_;
  OutputStream* output_;
  int indent_;
};


}
}
#endif
