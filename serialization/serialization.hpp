#ifndef SERIALIZATION
#define SERIALIZATION

#include <torch/torch.h>

namespace serialization {

    // Base class for serialization
    template<typename Derived>
    class Serializer {
    public:
        static std::unique_ptr<char[]> serialize(const torch::Tensor &t, uint64_t &buffer_size) {
            return Derived::serialize_impl(t, buffer_size);
        }

        static void deseralize(torch::Tensor &t, std::unique_ptr<char[]> buffer, const uint64_t buffer_size) {
            Derived::deseralize_impl(t, std::move(buffer), buffer_size);
        }
    };

    class InBuiltSerializer: public Serializer<InBuiltSerializer> {
    public:
        static std::unique_ptr<char[]> serialize_impl(const torch::Tensor &t, uint64_t &buffer_size);
        static void deseralize(torch::Tensor &t, std::unique_ptr<char[]> buffer, const uint64_t buffer_size);
    };

    class RawPtrSerializer: public Serializer<RawPtrSerializer> {
    public:
        static std::unique_ptr<char[]> serialize_impl(const torch::Tensor &t, uint64_t &buffer_size);
        static void deseralize(torch::Tensor &t, std::unique_ptr<char[]> buffer, const uint64_t buffer_size);
    };

} // namepsace serialization

#endif // SERIALIZATION