#include "containers/ring_buffer.hpp"
#include <vector>

template<typename T> class FIRFilter{

    private:
        static size_t validateSize(const std::vector<T>& coefficients)
        {
            if(coefficients.empty())
            {
                throw std::invalid_argument(
                    "Coefficients vector cannot be empty"
                );
            }

            return coefficients.size();
        }

        RingBuffer<T> history;
        std::vector<T> coefficients;
        
    public:
        explicit FIRFilter(const std::vector<T>& coefficients)
            : history(validateSize(coefficients)),
            coefficients(coefficients)
        {
        }   

        T processSample(T sample){

            history.write(sample);

            T output = T{};

            for(size_t i = 0; i < coefficients.size(); ++i)
            {
                output += coefficients[i] * history.getDelayed(i);
            }

            return output;
        }
};