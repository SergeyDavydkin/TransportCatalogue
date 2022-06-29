#pragma once

#include "json.h"

#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
#include <optional>
#include <string>
#include <vector>

namespace json {
    enum class Step {
        BUILD,
        ARR,
        DICT
    };

    class DictItemContext;    
    class DictKeyContext;
    class ArrayItemContext;

    class Builder {
    public:
        Builder();
        DictItemContext StartDict();
        Builder& EndDict();
        ArrayItemContext StartArray();
        Builder& EndArray();
        DictKeyContext Key(const std::string& key);
        Builder& Value(const Node::Value& val);
        Node Build() const;
        void AddNode(Node&& node);
    private:
        std::optional<Node> root_;
        std::vector<Step> step_stack_;
        std::vector< std::optional<std::string> > keys_;
        int dicts_open_ = 0;
        int arrays_open_ = 0;
        std::vector<std::vector<Node>> all_arrays_;
        std::vector< std::vector<std::pair<std::string, Node>> > all_dicts_;        
    };

    class DictItemContext {
    public:
        DictItemContext(Builder& builder);
        DictKeyContext Key(const std::string& key);
        Builder& EndDict();
    private:
        Builder& builder_;
    };

    class DictKeyContext {
    public:
        DictKeyContext(Builder& builder);
        DictItemContext Value(const Node::Value& val);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
    private:
        Builder& builder_;
    };

    class ArrayItemContext {
    public:
        ArrayItemContext(Builder& builder);
        ArrayItemContext Value(const Node::Value& val);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        Builder& EndArray();
    private:
        Builder& builder_;
    };
}// namespace json
