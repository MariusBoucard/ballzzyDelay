/* ------------------------------------------------------------
name: "FaustEffect"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -a faustMinimal.h -lang cpp -i -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h ******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***********************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>
#include <stdio.h>

/************************** BEGIN UI.h *****************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __UI_H__
#define __UI_H__

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***************************************************************************/

#ifndef __export__
#define __export__

// Version as a global string
#define FAUSTVERSION "2.83.1"

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 83
#define FAUSTPATCHVERSION 1

// Use FAUST_API for code that is part of the external API but is also compiled in faust and libfaust
// Use LIBFAUST_API for code that is compiled in faust and libfaust

#ifdef _WIN32
    #pragma warning (disable: 4251)
    #ifdef FAUST_EXE
        #define FAUST_API
        #define LIBFAUST_API
    #elif FAUST_LIB
        #define FAUST_API __declspec(dllexport)
        #define LIBFAUST_API __declspec(dllexport)
    #else
        #define FAUST_API
        #define LIBFAUST_API
    #endif
#else
    #ifdef FAUST_EXE
        #define FAUST_API
        #define LIBFAUST_API
    #else
        #define FAUST_API __attribute__((visibility("default")))
        #define LIBFAUST_API __attribute__((visibility("default")))
    #endif
#endif

#endif

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct FAUST_API UIReal {

    UIReal() {}
    virtual ~UIReal() {}

    // -- widget's layouts

    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;

    // -- active widgets

    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;

    // -- passive widgets

    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;

    // -- soundfiles

    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

    // -- metadata declarations

    virtual void declare(REAL* /*zone*/, const char* /*key*/, const char* /*val*/) {}

    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct FAUST_API UI : public UIReal<FAUSTFLOAT> {
    UI() {}
    virtual ~UI() {}
#ifdef DAISY_NO_RTTI
    virtual bool isSoundUI() const { return false; }
    virtual bool isMidiInterface() const { return false; }
#endif
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __PathBuilder__
#define __PathBuilder__

#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>


/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class FAUST_API PathBuilder {

    protected:

        std::vector<std::string> fControlsLevel;
        std::vector<std::string> fFullPaths;
        std::map<std::string, std::string> fFull2Short;  // filled by computeShortNames()

        /**
         * @brief check if a character is acceptable for an ID
         *
         * @param c
         * @return true is the character is acceptable for an ID
         */
        bool isIDChar(char c) const
        {
            return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9'));
        }

        /**
         * @brief remove all "/0x00" parts
         *
         * @param src
         * @return modified string
         */
        std::string remove0x00(const std::string& src_aux) const
        {
            std::string src = src_aux;
            std::string from = "/0x00";
            std::string to = "";
            size_t pos = std::string::npos;
            while ((pos = src.find(from)) && (pos != std::string::npos)) {
                src = src.replace(pos, from.length(), to);
            }
            return src;
        }

        /**
         * @brief replace all non ID char with '_' (one '_' may replace several non ID char)
         *
         * @param src
         * @return modified string
         */
        std::string str2ID(const std::string& src) const
        {
            std::string dst;
            bool need_underscore = false;
            for (char c : src) {
                if (isIDChar(c) || (c == '/')) {
                    if (need_underscore) {
                        dst.push_back('_');
                        need_underscore = false;
                    }
                    dst.push_back(c);
                } else {
                    need_underscore = true;
                }
            }
            return dst;
        }

        /**
         * @brief Keep only the last n slash-parts
         *
         * @param src
         * @param n : 1 indicates the last slash-part
         * @return modified string
         */
        std::string cut(const std::string& src, int n) const
        {
            std::string rdst;
            for (int i = int(src.length())-1; i >= 0; i--) {
                char c = src[i];
                if (c != '/') {
                    rdst.push_back(c);
                } else if (n == 1) {
                    std::string dst;
                    for (int j = int(rdst.length())-1; j >= 0; j--) {
                        dst.push_back(rdst[j]);
                    }
                    return dst;
                } else {
                    n--;
                    rdst.push_back(c);
                }
            }
            return src;
        }

        void addFullPath(const std::string& label) { fFullPaths.push_back(buildPath(label)); }

        /**
         * @brief Compute the mapping between full path and short names
         */
        void computeShortNames()
        {
            std::vector<std::string>           uniquePaths;  // all full paths transformed but made unique with a prefix
            std::map<std::string, std::string> unique2full;  // all full paths transformed but made unique with a prefix
            char num_buffer[16];
            int pnum = 0;

            for (const auto& s : fFullPaths) {
                // Using snprintf since Teensy does not have the std::to_string function
                snprintf(num_buffer, 16, "%d", pnum++);
                std::string u = "/P" + std::string(num_buffer) + str2ID(remove0x00(s));
                uniquePaths.push_back(u);
                unique2full[u] = s;  // remember the full path associated to a unique path
            }

            std::map<std::string, int> uniquePath2level;                // map path to level
            for (const auto& s : uniquePaths) uniquePath2level[s] = 1;   // we init all levels to 1
            bool have_collisions = true;

            while (have_collisions) {
                // compute collision list
                std::set<std::string>              collisionSet;
                std::map<std::string, std::string> short2full;
                have_collisions = false;
                for (const auto& it : uniquePath2level) {
                    std::string u = it.first;
                    int n = it.second;
                    std::string shortName = cut(u, n);
                    auto p = short2full.find(shortName);
                    if (p == short2full.end()) {
                        // no collision
                        short2full[shortName] = u;
                    } else {
                        // we have a collision, add the two paths to the collision set
                        have_collisions = true;
                        collisionSet.insert(u);
                        collisionSet.insert(p->second);
                    }
                }
                for (const auto& s : collisionSet) uniquePath2level[s]++;  // increase level of colliding path
            }

            for (const auto& it : uniquePath2level) {
                std::string u = it.first;
                int n = it.second;
                std::string shortName = replaceCharList(cut(u, n), {'/'}, '_');
                fFull2Short[unique2full[u]] = shortName;
            }
        }

        std::string replaceCharList(const std::string& str, const std::vector<char>& ch1, char ch2)
        {
            auto beg = ch1.begin();
            auto end = ch1.end();
            std::string res = str;
            for (size_t i = 0; i < str.length(); ++i) {
                if (std::find(beg, end, str[i]) != end) res[i] = ch2;
            }
            return res;
        }

    public:

        PathBuilder() {}
        virtual ~PathBuilder() {}

        // Return true for the first level of groups
        bool pushLabel(const std::string& label_aux)
        {
            std::string label = replaceCharList(label_aux, {'/'}, '_');
            fControlsLevel.push_back(label); return fControlsLevel.size() == 1;
        }

        // Return true for the last level of groups
        bool popLabel() { fControlsLevel.pop_back(); return fControlsLevel.size() == 0; }

        // Return a complete path built from a label
        std::string buildPath(const std::string& label_aux)
        {
            std::string label = replaceCharList(label_aux, {'/'}, '_');
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res = res + fControlsLevel[i] + "/";
            }
            res += label;
            return replaceCharList(res, {' ', '#', '*', ',', '?', '[', ']', '{', '}', '(', ')'}, '_');
        }

        // Assuming shortnames have been built, return the shortname from a label
        std::string buildShortname(const std::string& label)
        {
            return (hasShortname()) ? fFull2Short[buildPath(label)] : "";
        }

        bool hasShortname() { return fFull2Short.size() > 0; }

};

#endif  // __PathBuilder__
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface.
 *
 * This class creates:
 * - a map of 'labels' and zones for each UI item.
 * - a map of unique 'shortname' (built so that they never collide) and zones for each UI item
 * - a map of complete hierarchical 'paths' and zones for each UI item
 *
 * Simple 'labels', 'shortname' and complete 'paths' (to fully discriminate between possible same
 * 'labels' at different location in the UI hierachy) can be used to access a given parameter.
 ******************************************************************************/

class FAUST_API MapUI : public UI, public PathBuilder
{

    protected:

        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;

        // Shortname zone map
        std::map<std::string, FAUSTFLOAT*> fShortnameZoneMap;

        // Full path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;

        void addZoneLabel(const std::string& label, FAUSTFLOAT* zone)
        {
            std::string path = buildPath(label);
            fFullPaths.push_back(path);
            fPathZoneMap[path] = zone;
            fLabelZoneMap[label] = zone;
        }

    public:

        MapUI() {}
        virtual ~MapUI() {}

        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            if (popLabel()) {
                // Shortnames can be computed when all fullnames are known
                computeShortNames();
                // Fill 'shortname' map
                for (const auto& it : fFullPaths) {
                    fShortnameZoneMap[fFull2Short[it]] = fPathZoneMap[it];
                }
            }
        }

        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            addZoneLabel(label, zone);
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            addZoneLabel(label, zone);
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }

        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            addZoneLabel(label, zone);
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            addZoneLabel(label, zone);
        }

        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

        // -- metadata declarations
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}

        //-------------------------------------------------------------------------------
        // Public API
        //-------------------------------------------------------------------------------

        /**
         * Set the param value.
         *
         * @param str - the UI parameter label/shortname/path
         * @param value - the UI parameter value
         *
         */
        void setParamValue(const std::string& str, FAUSTFLOAT value)
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                *fPathZoneMapIter->second = value;
                return;
            }

            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                *fShortnameZoneMapIter->second = value;
                return;
            }

            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                *fLabelZoneMapIter->second = value;
                return;
            }

            fprintf(stderr, "ERROR : setParamValue '%s' not found\n", str.c_str());
        }

        /**
         * Return the param value.
         *
         * @param str - the UI parameter label/shortname/path
         *
         * @return the param value.
         */
        FAUSTFLOAT getParamValue(const std::string& str) const
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                return *fPathZoneMapIter->second;
            }

            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                return *fShortnameZoneMapIter->second;
            }

            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                return *fLabelZoneMapIter->second;
            }

            fprintf(stderr, "ERROR : getParamValue '%s' not found\n", str.c_str());
            return 0;
        }

        // map access
        std::map<std::string, FAUSTFLOAT*>& getFullpathMap() { return fPathZoneMap; }
        std::map<std::string, FAUSTFLOAT*>& getShortnameMap() { return fShortnameZoneMap; }
        std::map<std::string, FAUSTFLOAT*>& getLabelMap() { return fLabelZoneMap; }

        /**
         * Return the number of parameters in the UI.
         *
         * @return the number of parameters
         */
        int getParamsCount() const { return int(fPathZoneMap.size()); }

        /**
         * Return the param path.
         *
         * @param index - the UI parameter index
         *
         * @return the param path
         */
        std::string getParamAddress(int index) const
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return "";
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }

        const char* getParamAddress1(int index) const
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first.c_str();
            }
        }

        /**
         * Return the param shortname.
         *
         * @param index - the UI parameter index
         *
         * @return the param shortname
         */
        std::string getParamShortname(int index) const
        {
            if (index < 0 || index > int(fShortnameZoneMap.size())) {
                return "";
            } else {
                auto it = fShortnameZoneMap.begin();
                while (index-- > 0 && it++ != fShortnameZoneMap.end()) {}
                return it->first;
            }
        }

        const char* getParamShortname1(int index) const
        {
            if (index < 0 || index > int(fShortnameZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fShortnameZoneMap.begin();
                while (index-- > 0 && it++ != fShortnameZoneMap.end()) {}
                return it->first.c_str();
            }
        }

        /**
         * Return the param label.
         *
         * @param index - the UI parameter index
         *
         * @return the param label
         */
        std::string getParamLabel(int index) const
        {
            if (index < 0 || index > int(fLabelZoneMap.size())) {
                return "";
            } else {
                auto it = fLabelZoneMap.begin();
                while (index-- > 0 && it++ != fLabelZoneMap.end()) {}
                return it->first;
            }
        }

        const char* getParamLabel1(int index) const
        {
            if (index < 0 || index > int(fLabelZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fLabelZoneMap.begin();
                while (index-- > 0 && it++ != fLabelZoneMap.end()) {}
                return it->first.c_str();
            }
        }

        /**
         * Return the param path.
         *
         * @param zone - the UI parameter memory zone
         *
         * @return the param path
         */
        std::string getParamAddress(FAUSTFLOAT* zone) const
        {
            for (const auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return "";
        }

        /**
         * Return the param memory zone.
         *
         * @param zone - the UI parameter label/shortname/path
         *
         * @return the param path
         */
        FAUSTFLOAT* getParamZone(const std::string& str) const
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                return fPathZoneMapIter->second;
            }

            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                return fShortnameZoneMapIter->second;
            }

            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                return fLabelZoneMapIter->second;
            }

            return nullptr;
        }

        /**
         * Return the param memory zone.
         *
         * @param zone - the UI parameter index
         *
         * @return the param path
         */
        FAUSTFLOAT* getParamZone(int index) const
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }

        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }

};

#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h *******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__


/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct FAUST_API Meta {
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h ********************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>
#include <cstdint>


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct FAUST_API UI;
struct FAUST_API Meta;

/**
 * DSP memory manager.
 */

struct FAUST_API dsp_memory_manager {

    enum MemType { kInt32, kInt32_ptr, kFloat, kFloat_ptr, kDouble, kDouble_ptr, kQuad, kQuad_ptr, kFixedPoint, kFixedPoint_ptr, kObj, kObj_ptr, kSound, kSound_ptr };

    virtual ~dsp_memory_manager() = default;

    /**
     * Inform the Memory Manager with the number of expected memory zones.
     * @param count - the number of expected memory zones
     */
    virtual void begin(size_t count) {}

    /**
     * Give the Memory Manager information on a given memory zone.
     * @param name - the memory zone name
     * @param type - the memory zone type (in MemType)
     * @param size - the size in unit of the memory type of the memory zone
     * @param size_bytes - the size in bytes of the memory zone
     * @param reads - the number of Read access to the zone used to compute one frame
     * @param writes - the number of Write access to the zone used to compute one frame
     */
    virtual void info(const char* name, MemType type, size_t size, size_t size_bytes, size_t reads, size_t writes) {}

    /**
     * Inform the Memory Manager that all memory zones have been described,
     * to possibly start a 'compute the best allocation strategy' step.
     */
    virtual void end() {}

    /**
     * Allocate a memory zone.
     * @param size - the memory zone size in bytes
     */
    virtual void* allocate(size_t size) = 0;

    /**
     * Destroy a memory zone.
     * @param ptr - the memory zone pointer to be deallocated
     */
    virtual void destroy(void* ptr) = 0;

};

/**
* Signal processor definition.
*/

class FAUST_API dsp {

    public:

        dsp() = default;
        virtual ~dsp() = default;

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;

        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;

        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;

        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;

        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state.
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state.
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;

        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;

        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;

        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual ::dsp* clone() = 0;

        /**
         * Trigger the Meta* m parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;

        /**
         * Read all controllers (buttons, sliders, etc.), and update the DSP state to be used by 'frame' or 'compute'.
         * This method will be filled with the -ec (--external-control) option.
         */
        virtual void control() {}

        /**
         * DSP instance computation to process one single frame.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write frame(inputs, inputs).
         * The -inpl option can be used for that, but only in scalar mode for now.
         * This method will be filled with the -os (--one-sample) option.
         *
         * @param inputs - the input audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         */
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) {}

        /**
         * DSP instance computation to be called with successive in/out audio buffers.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write compute(count, inputs, inputs).
         * The -inpl compilation option can be used for that, but only in scalar mode for now.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;

        /**
         * Alternative DSP instance computation method for use by subclasses, incorporating an additional `date_usec` parameter,
         * which specifies the timestamp of the first sample in the audio buffers.
         *
         * @param date_usec - the timestamp in microsec given by audio driver. By convention timestamp of -1 means 'no timestamp conversion',
         * events already have a timestamp expressed in frames.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

/**
 * Generic DSP decorator.
 */

class FAUST_API decorator_dsp : public ::dsp {

    protected:

        ::dsp* fDSP;

    public:

        decorator_dsp(::dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() override { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() override { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) override { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() override { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) override { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) override { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) override { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() override { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() override { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() override { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) override { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void control() override { fDSP->control(); }
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) override { fDSP->frame(inputs, outputs); }
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) override { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) override { fDSP->compute(date_usec, count, inputs, outputs); }

};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class FAUST_API dsp_factory {

    protected:

        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() = default;

    public:

        /* Return factory name */
        virtual std::string getName() = 0;

        /* Return factory SHA key */
        virtual std::string getSHAKey() = 0;

        /* Return factory expanded DSP code */
        virtual std::string getDSPCode() = 0;

        /* Return factory compile options */
        virtual std::string getCompileOptions() = 0;

        /* Get the Faust DSP factory list of library dependancies */
        virtual std::vector<std::string> getLibraryList() = 0;

        /* Get the list of all used includes */
        virtual std::vector<std::string> getIncludePathnames() = 0;

        /* Get warning messages list for a given compilation */
        virtual std::vector<std::string> getWarningMessages() = 0;

        /* Create a new DSP instance, to be deleted with C++ 'delete' */
        virtual ::dsp* createDSPInstance() = 0;

        /* Static tables initialization, possibly implemened in sub-classes*/
        virtual void classInit(int sample_rate) {};

        /* Set a custom memory manager to be used when creating instances */
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;

        /* Return the currently set custom memory manager */
        virtual dsp_memory_manager* getMemoryManager() = 0;

};

// Denormal handling

#if defined (__SSE__)
#include <xmmintrin.h>
#endif

class FAUST_API ScopedNoDenormals {

    private:

        intptr_t fpsr = 0;

        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            // The volatile keyword here is needed to workaround a bug in AppleClang 13.0
            // which aggressively optimises away the variable otherwise
            volatile uint32_t fpsr_w = static_cast<uint32_t>(fpsr_aux);
            _mm_setcsr(fpsr_w);
        #endif
        }

        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined (__SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }

    public:

        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #elif defined (__SSE__)
        #if defined (__SSE2__)
            intptr_t mask = 0x8040;
        #else
            intptr_t mask = 0x8000;
        #endif
        #else
            intptr_t mask = 0x0000;
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }

        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals ftz_scope;

#endif

/************************** END dsp.h **************************/

// BEGIN-FAUSTDSP


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif


class mydsp : public dsp {

 private:

	FAUSTFLOAT fVslider0;
	int IOTA0;
	float fVec0[1048576];
	int fSampleRate;
	float fConst0;
	int iConst1;
	FAUSTFLOAT fVslider1;
	float fRec1[2];
	float fRec2[2];
	int iRec3[2];
	int iRec4[2];
	float fRec0[2];
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	FAUSTFLOAT fVslider4;
	float fVec1[1048576];
	FAUSTFLOAT fVslider5;
	float fRec6[2];
	float fRec7[2];
	int iRec8[2];
	int iRec9[2];
	float fRec5[2];
	FAUSTFLOAT fVslider6;
	FAUSTFLOAT fVslider7;
	FAUSTFLOAT fVslider8;
	float fVec2[1048576];
	FAUSTFLOAT fVslider9;
	float fRec11[2];
	float fRec12[2];
	int iRec13[2];
	int iRec14[2];
	float fRec10[2];
	FAUSTFLOAT fVslider10;
	FAUSTFLOAT fVslider11;
	FAUSTFLOAT fVslider12;
	float fVec3[1048576];
	FAUSTFLOAT fVslider13;
	float fRec16[2];
	float fRec17[2];
	int iRec18[2];
	int iRec19[2];
	float fRec15[2];
	FAUSTFLOAT fVslider14;
	FAUSTFLOAT fVslider15;
	FAUSTFLOAT fHslider0;

 public:
	mydsp() {
	}

	mydsp(const mydsp&) = default;

	virtual ~mydsp() = default;

	mydsp& operator=(const mydsp&) = default;

	void metadata(Meta* m) {
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-a faustMinimal.h -lang cpp -i -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "FaustEffect.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "FaustEffect");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("spats.lib/name", "Faust Spatialization Library");
		m->declare("spats.lib/version", "1.2.1");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 2;
	}

	static void classInit(int sample_rate) {
	}

	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		iConst1 = static_cast<int>(4.0f * fConst0);
	}

	virtual void instanceResetUserInterface() {
		fVslider0 = static_cast<FAUSTFLOAT>(0.3f);
		fVslider1 = static_cast<FAUSTFLOAT>(0.8f);
		fVslider2 = static_cast<FAUSTFLOAT>(0.5f);
		fVslider3 = static_cast<FAUSTFLOAT>(0.25f);
		fVslider4 = static_cast<FAUSTFLOAT>(0.3f);
		fVslider5 = static_cast<FAUSTFLOAT>(0.6f);
		fVslider6 = static_cast<FAUSTFLOAT>(0.5f);
		fVslider7 = static_cast<FAUSTFLOAT>(0.25f);
		fVslider8 = static_cast<FAUSTFLOAT>(0.3f);
		fVslider9 = static_cast<FAUSTFLOAT>(0.4f);
		fVslider10 = static_cast<FAUSTFLOAT>(0.5f);
		fVslider11 = static_cast<FAUSTFLOAT>(0.25f);
		fVslider12 = static_cast<FAUSTFLOAT>(0.3f);
		fVslider13 = static_cast<FAUSTFLOAT>(0.2f);
		fVslider14 = static_cast<FAUSTFLOAT>(0.5f);
		fVslider15 = static_cast<FAUSTFLOAT>(0.25f);
		fHslider0 = static_cast<FAUSTFLOAT>(0.5f);
	}

	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 1048576; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec3[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec4[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec0[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 1048576; l6 = l6 + 1) {
			fVec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			iRec8[l9] = 0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			iRec9[l10] = 0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec5[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 1048576; l12 = l12 + 1) {
			fVec2[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			iRec13[l15] = 0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			iRec14[l16] = 0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec10[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 1048576; l18 = l18 + 1) {
			fVec3[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			iRec18[l21] = 0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			iRec19[l22] = 0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec15[l23] = 0.0f;
		}
	}

	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}

	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}

	virtual mydsp* clone() {
		return new mydsp(*this);
	}

	virtual int getSampleRate() {
		return fSampleRate;
	}

	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("FaustEffect");
		ui_interface->openHorizontalBox("Delay Multi-Tap");
		ui_interface->openVerticalBox("Head 0");
		ui_interface->declare(&fVslider12, "style", "knob");
		ui_interface->addVerticalSlider("Feedback", &fVslider12, FAUSTFLOAT(0.3f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.95f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider15, "style", "knob");
		ui_interface->addVerticalSlider("Level", &fVslider15, FAUSTFLOAT(0.25f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider14, "style", "knob");
		ui_interface->addVerticalSlider("Pan", &fVslider14, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider13, "style", "knob");
		ui_interface->declare(&fVslider13, "unit", "s");
		ui_interface->addVerticalSlider("Time", &fVslider13, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Head 1");
		ui_interface->declare(&fVslider8, "style", "knob");
		ui_interface->addVerticalSlider("Feedback", &fVslider8, FAUSTFLOAT(0.3f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.95f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider11, "style", "knob");
		ui_interface->addVerticalSlider("Level", &fVslider11, FAUSTFLOAT(0.25f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider10, "style", "knob");
		ui_interface->addVerticalSlider("Pan", &fVslider10, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider9, "style", "knob");
		ui_interface->declare(&fVslider9, "unit", "s");
		ui_interface->addVerticalSlider("Time", &fVslider9, FAUSTFLOAT(0.4f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Head 2");
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->addVerticalSlider("Feedback", &fVslider4, FAUSTFLOAT(0.3f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.95f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider7, "style", "knob");
		ui_interface->addVerticalSlider("Level", &fVslider7, FAUSTFLOAT(0.25f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider6, "style", "knob");
		ui_interface->addVerticalSlider("Pan", &fVslider6, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider5, "style", "knob");
		ui_interface->declare(&fVslider5, "unit", "s");
		ui_interface->addVerticalSlider("Time", &fVslider5, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Head 3");
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->addVerticalSlider("Feedback", &fVslider0, FAUSTFLOAT(0.3f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.95f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->addVerticalSlider("Level", &fVslider3, FAUSTFLOAT(0.25f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->addVerticalSlider("Pan", &fVslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "s");
		ui_interface->addVerticalSlider("Time", &fVslider1, FAUSTFLOAT(0.8f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Global Settings");
		ui_interface->addHorizontalSlider("Global Mix", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
		ui_interface->closeBox();
	}

	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = static_cast<float>(fVslider0);
		int iSlow1 = static_cast<int>(fConst0 * static_cast<float>(fVslider1));
		float fSlow2 = static_cast<float>(fVslider2);
		float fSlow3 = static_cast<float>(fVslider3);
		float fSlow4 = fSlow3 * (1.0f - fSlow2);
		float fSlow5 = static_cast<float>(fVslider4);
		int iSlow6 = static_cast<int>(fConst0 * static_cast<float>(fVslider5));
		float fSlow7 = static_cast<float>(fVslider6);
		float fSlow8 = static_cast<float>(fVslider7);
		float fSlow9 = fSlow8 * (1.0f - fSlow7);
		float fSlow10 = static_cast<float>(fVslider8);
		int iSlow11 = static_cast<int>(fConst0 * static_cast<float>(fVslider9));
		float fSlow12 = static_cast<float>(fVslider10);
		float fSlow13 = static_cast<float>(fVslider11);
		float fSlow14 = fSlow13 * (1.0f - fSlow12);
		float fSlow15 = static_cast<float>(fVslider12);
		int iSlow16 = static_cast<int>(fConst0 * static_cast<float>(fVslider13));
		float fSlow17 = static_cast<float>(fVslider14);
		float fSlow18 = static_cast<float>(fVslider15);
		float fSlow19 = fSlow18 * (1.0f - fSlow17);
		float fSlow20 = static_cast<float>(fHslider0);
		float fSlow21 = 1.0f - fSlow20;
		float fSlow22 = fSlow2 * fSlow3;
		float fSlow23 = fSlow7 * fSlow8;
		float fSlow24 = fSlow12 * fSlow13;
		float fSlow25 = fSlow17 * fSlow18;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = static_cast<float>(input0[i0]);
			float fTemp1 = fTemp0 + fSlow0 * fRec0[1];
			fVec0[IOTA0 & 1048575] = fTemp1;
			float fTemp2 = ((fRec1[1] != 0.0f) ? (((fRec2[1] > 0.0f) & (fRec2[1] < 1.0f)) ? fRec1[1] : 0.0f) : (((fRec2[1] == 0.0f) & (iSlow1 != iRec3[1])) ? 0.0009765625f : (((fRec2[1] == 1.0f) & (iSlow1 != iRec4[1])) ? -0.0009765625f : 0.0f)));
			fRec1[0] = fTemp2;
			fRec2[0] = std::max<float>(0.0f, std::min<float>(1.0f, fRec2[1] + fTemp2));
			iRec3[0] = (((fRec2[1] >= 1.0f) & (iRec4[1] != iSlow1)) ? iSlow1 : iRec3[1]);
			iRec4[0] = (((fRec2[1] <= 0.0f) & (iRec3[1] != iSlow1)) ? iSlow1 : iRec4[1]);
			float fTemp3 = fVec0[(IOTA0 - std::min<int>(iConst1, std::max<int>(0, iRec3[0]))) & 1048575];
			fRec0[0] = fTemp3 + fRec2[0] * (fVec0[(IOTA0 - std::min<int>(iConst1, std::max<int>(0, iRec4[0]))) & 1048575] - fTemp3);
			float fTemp4 = fTemp0 + fSlow5 * fRec5[1];
			fVec1[IOTA0 & 1048575] = fTemp4;
			float fTemp5 = ((fRec6[1] != 0.0f) ? (((fRec7[1] > 0.0f) & (fRec7[1] < 1.0f)) ? fRec6[1] : 0.0f) : (((fRec7[1] == 0.0f) & (iSlow6 != iRec8[1])) ? 0.0009765625f : (((fRec7[1] == 1.0f) & (iSlow6 != iRec9[1])) ? -0.0009765625f : 0.0f)));
			fRec6[0] = fTemp5;
			fRec7[0] = std::max<float>(0.0f, std::min<float>(1.0f, fRec7[1] + fTemp5));
			iRec8[0] = (((fRec7[1] >= 1.0f) & (iRec9[1] != iSlow6)) ? iSlow6 : iRec8[1]);
			iRec9[0] = (((fRec7[1] <= 0.0f) & (iRec8[1] != iSlow6)) ? iSlow6 : iRec9[1]);
			float fTemp6 = fVec1[(IOTA0 - std::min<int>(iConst1, std::max<int>(0, iRec8[0]))) & 1048575];
			fRec5[0] = fTemp6 + fRec7[0] * (fVec1[(IOTA0 - std::min<int>(iConst1, std::max<int>(0, iRec9[0]))) & 1048575] - fTemp6);
			float fTemp7 = fTemp0 + fSlow10 * fRec10[1];
			fVec2[IOTA0 & 1048575] = fTemp7;
			float fTemp8 = ((fRec11[1] != 0.0f) ? (((fRec12[1] > 0.0f) & (fRec12[1] < 1.0f)) ? fRec11[1] : 0.0f) : (((fRec12[1] == 0.0f) & (iSlow11 != iRec13[1])) ? 0.0009765625f : (((fRec12[1] == 1.0f) & (iSlow11 != iRec14[1])) ? -0.0009765625f : 0.0f)));
			fRec11[0] = fTemp8;
			fRec12[0] = std::max<float>(0.0f, std::min<float>(1.0f, fRec12[1] + fTemp8));
			iRec13[0] = (((fRec12[1] >= 1.0f) & (iRec14[1] != iSlow11)) ? iSlow11 : iRec13[1]);
			iRec14[0] = (((fRec12[1] <= 0.0f) & (iRec13[1] != iSlow11)) ? iSlow11 : iRec14[1]);
			float fTemp9 = fVec2[(IOTA0 - std::min<int>(iConst1, std::max<int>(0, iRec13[0]))) & 1048575];
			fRec10[0] = fTemp9 + fRec12[0] * (fVec2[(IOTA0 - std::min<int>(iConst1, std::max<int>(0, iRec14[0]))) & 1048575] - fTemp9);
			float fTemp10 = fTemp0 + fSlow15 * fRec15[1];
			fVec3[IOTA0 & 1048575] = fTemp10;
			float fTemp11 = ((fRec16[1] != 0.0f) ? (((fRec17[1] > 0.0f) & (fRec17[1] < 1.0f)) ? fRec16[1] : 0.0f) : (((fRec17[1] == 0.0f) & (iSlow16 != iRec18[1])) ? 0.0009765625f : (((fRec17[1] == 1.0f) & (iSlow16 != iRec19[1])) ? -0.0009765625f : 0.0f)));
			fRec16[0] = fTemp11;
			fRec17[0] = std::max<float>(0.0f, std::min<float>(1.0f, fRec17[1] + fTemp11));
			iRec18[0] = (((fRec17[1] >= 1.0f) & (iRec19[1] != iSlow16)) ? iSlow16 : iRec18[1]);
			iRec19[0] = (((fRec17[1] <= 0.0f) & (iRec18[1] != iSlow16)) ? iSlow16 : iRec19[1]);
			float fTemp12 = fVec3[(IOTA0 - std::min<int>(iConst1, std::max<int>(0, iRec18[0]))) & 1048575];
			fRec15[0] = fTemp12 + fRec17[0] * (fVec3[(IOTA0 - std::min<int>(iConst1, std::max<int>(0, iRec19[0]))) & 1048575] - fTemp12);
			float fTemp13 = fSlow21 * fTemp0;
			output0[i0] = static_cast<FAUSTFLOAT>(fTemp13 + fSlow20 * (fSlow19 * fRec15[0] + fSlow14 * fRec10[0] + fSlow9 * fRec5[0] + fSlow4 * fRec0[0]));
			output1[i0] = static_cast<FAUSTFLOAT>(fTemp13 + fSlow20 * (fSlow25 * fRec15[0] + fSlow24 * fRec10[0] + fSlow23 * fRec5[0] + fSlow22 * fRec0[0]));
			IOTA0 = IOTA0 + 1;
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			iRec3[1] = iRec3[0];
			iRec4[1] = iRec4[0];
			fRec0[1] = fRec0[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			iRec8[1] = iRec8[0];
			iRec9[1] = iRec9[0];
			fRec5[1] = fRec5[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			iRec13[1] = iRec13[0];
			iRec14[1] = iRec14[0];
			fRec10[1] = fRec10[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			iRec18[1] = iRec18[0];
			iRec19[1] = iRec19[0];
			fRec15[1] = fRec15[0];
		}
	}

};

// END-FAUSTDSP

#endif
