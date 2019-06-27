#pragma once

#include <type_traits>
#include <vector>

namespace agz::math::distribution
{
    
/**
 * @brief 将[0, 1]间的浮点数u转换为[beg, end)中的整数
 */
template<typename T, typename F, typename = std::enable_if_t<std::is_integral_v<T> && std::is_floating_point_v<F>>>
T uniform_integer(T beg, T end, F u);

template<typename F, typename T = int>
class bsearch_sampler_t
{
    static_assert(std::is_floating_point_v<F> && std::is_integral_v<T>);

    std::vector<F> partial_sum_;

public:

    bsearch_sampler_t() = default;

    bsearch_sampler_t(const F *prob, T n);

    /**
     * @param prob 与分布律成固定比例的数组
     * @param n 数组大小
     *
     * 应满足prob[i] >= 0
     */
    void initialize(const F *prob, T n);

    bool available() const noexcept;

    void destroy();

    /**
     * @brief 以归一化后的prob数组为分布律采样一个1到n-1间的整数
     * @param u [0, 1]间的均匀随机数
     */
    T sample(F u) const noexcept;
};

/**
 * @brief 常数时间按离散分布律进行采样
 * 
 * 参见 https://en.wikipedia.org/wiki/Alias_methods
 */
template<typename F, typename T = int>
class alias_sampler_t
{
    static_assert(std::is_floating_point_v<F> && std::is_integral_v<T>);

    struct table_unit
    {
        F accept_prob;
        T another_idx;
    };

    std::vector<table_unit> table_;

public:

    alias_sampler_t() = default;

    alias_sampler_t(const F *prob, T n);

    /**
     * @param prob 与分布律成固定比例的数组
     * @param n 数组大小
     * 
     * 应满足prob[i] >= 0
     */
    void initialize(const F *prob, T n);

    bool available() const noexcept;

    void destroy();

    /**
     * @brief 以归一化后的prob数组为分布律采样一个1到n-1间的整数
     * @param u [0, 1]间的均匀随机数
     */
    T sample(F u) const noexcept;

    /**
     * @brief 以归一化后的prob数组为分布律采样一个1到n-1间的整数
     * @param u1 [0, 1]间的均匀随机数
     * @param u2 [0, 1]间的均匀随机数
     */
    T sample(F u1, F u2) const noexcept;
};

/**
 * @brief 将[0, 1]^2上的均匀分布转换为单位球面上的均匀分布（w.r.t. solid angle）
 */
template<typename F>
std::pair<tvec3<F>, F> uniform_on_sphere(F u1, F u2) noexcept;

/**
 * @brief 单位球面上的均匀分布对应的pdf（w.r.t. solid angle）
 */
template<typename F>
constexpr F uniform_on_sphere_pdf = 1 / (4 * PI<F>);

/**
 * @brief 将[0, 1]^2上的均匀分布转换为单位半球面上的均匀分布（w.r.t. solid angle）
 * 
 * （+z那一侧的单位球面）
 */
template<typename F>
std::pair<tvec3<F>, F> uniform_on_hemisphere(F u1, F u2) noexcept;

/**
 * @brief 单位半球面上的均匀分布对应的pdf（w.r.t. solid angle）
 */
template<typename F>
constexpr F uniform_on_hemisphere_pdf = 1 / (2 * PI<F>);

/**
 * @brief 把[0, 1]^2上的均匀分布转换为以maxCosTheta为顶角的单位球面上的锥体内的方向上的均匀分布（w.r.t. solid angle）
 * 
 * @param max_cos_theta 以+z为锥体中心，theta是方向向量与+z的夹角
 */
template<typename F>
std::pair<tvec3<F>, F> uniform_on_cone(F max_cos_theta, F u1, F u2) noexcept;

/**
 * @brief 把[0, 1]^2上的均匀分布转换为以maxCosTheta为顶角的单位球面上的锥体内的方向上的均匀分布对应的pdf（w.r.t. solid angle）
 */
template<typename F>
F uniform_on_cone_pdf(F max_cos_theta) noexcept;

/**
 * @brief 将[0, 1]^2上的均匀分布转换为单位半球面上以z为权重的分布（w.r.t. solid angle）
 * 
 * （+z那一侧的单位球面）
 */
template<typename F>
std::pair<tvec3<F>, F> zweighted_on_hemisphere(F u1, F u2) noexcept;

/**
 * @brief 单位半球面上以z为权重的分布对应的pdf（w.r.t. solid angle）
 */
template<typename F>
F zweighted_on_hemisphere_pdf(F z) noexcept;

/**
 * @brief 将[0, 1]^2上的均匀分布转换为三角形面片上的均匀分布，返回barycentric coordinate
 */
template<typename F>
tvec2<F> uniform_on_triangle(F u1, F u2) noexcept;

} // namespace agz::math::distribution
