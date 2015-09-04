
/*  
 Copyright (C) 2005, 2006 Plamen Neykov
 Copyright (C) 2007, 2008 Eric Ehlers
 
 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef rp_vo_range_hpp
#define rp_vo_range_hpp

#include <rp/valueobject.hpp>
#include <string>
#include <vector>
#include <set>
#include <boost/serialization/map.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace reposit { namespace ValueObjects {

    class rpRange : public reposit::ValueObject {
        friend class boost::serialization::access;
    public:
        rpRange() {}
        rpRange(
            const std::string& ObjectId,
            const std::vector<std::vector<double> >& Values,
            bool Permanent);

        const std::set<std::string>& getSystemPropertyNames() const;
        std::vector<std::string> getPropertyNamesVector() const;
        reposit::property_t getSystemProperty(const std::string&) const;
        void setSystemProperty(const std::string& name, const reposit::property_t& value);

    protected:

        std::vector<std::vector <double> > Values_;
        bool Permanent_;

        
        template<class Archive>
        void serialize(Archive& ar, const unsigned int) {
        boost::serialization::void_cast_register<rpRange, reposit::ValueObject>(this, this);
            ar  & boost::serialization::make_nvp("ObjectId", objectId_)
                & boost::serialization::make_nvp("ClassName", className_)
                & boost::serialization::make_nvp("Values", Values_)
                & boost::serialization::make_nvp("Permanent", Permanent_)
                & boost::serialization::make_nvp("UserProperties", userProperties);
        }
    };



    inline const std::set<std::string> &rpRange::getSystemPropertyNames() const {
        static std::set<std::string> ret;
        if (ret.empty()) {
        ret.insert("Values");
        ret.insert("Permanent");
        }
        return ret;
    }

    inline std::vector<std::string> rpRange::getPropertyNamesVector() const {
        std::vector<std::string> ret;
        ret.push_back("Values");
        ret.push_back("Permanent");
        for (std::map<std::string, reposit::property_t>::const_iterator i = userProperties.begin();
            i != userProperties.end(); ++i)
            ret.push_back(i->first);
        return ret;
    }

    inline reposit::property_t rpRange::getSystemProperty(const std::string& name) const {
        std::string nameUpper = boost::algorithm::to_upper_copy(name);
        if(strcmp(nameUpper.c_str(), "OBJECTID")==0)
            return objectId_;
        else if(strcmp(nameUpper.c_str(), "CLASSNAME")==0)
            return className_;
        else if(strcmp(nameUpper.c_str(), "VALUES")==0)
            return Values_;
        else if(strcmp(nameUpper.c_str(), "PERMANENT")==0)
            return Permanent_;
        else
            RP_FAIL("Error: attempt to retrieve non-existent Property: '" + name + "'");
    }

    inline void rpRange::setSystemProperty(const std::string& name, const reposit::property_t& value) {
        std::string nameUpper = boost::algorithm::to_upper_copy(name);
        if(strcmp(nameUpper.c_str(), "OBJECTID")==0)
            objectId_ = boost::get<std::string>(value);
        else if(strcmp(nameUpper.c_str(), "CLASSNAME")==0)
            className_ = boost::get<std::string>(value);
        else if(strcmp(nameUpper.c_str(), "VALUES")==0)
            Values_ = reposit::matrix::convert2<double>(value, nameUpper);
        else if(strcmp(nameUpper.c_str(), "PERMANENT")==0)
            Permanent_ = reposit::convert2<bool>(value);
        else
            RP_FAIL("Error: attempt to set non-existent Property: '" + name + "'");
    }

    inline rpRange::rpRange(
            const std::string& ObjectId,
            const std::vector<std::vector<double> >& Values,
            bool Permanent) :
        reposit::ValueObject(ObjectId, "rpRange", Permanent),
        Values_(Values),
        Permanent_(Permanent) {
    }

} }

#endif

