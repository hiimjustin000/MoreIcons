

class LogLayer : public geode::Popup<> {
protected:
    bool setup() override;
public:
    static LogLayer* create();
};
