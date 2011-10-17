#include <stdafx.h>

#include <my/gl/math/Vector4.h>

namespace my { namespace gl { namespace math {

	Vector4::Vector4 (float const x, float const y, float const z, float const w):
#pragma warning( push )
#pragma warning( disable: 4351 ) // elements of "v" will be default initialised
		v() {
#pragma warning( pop )
		P_STATIC_ASSERT( sizeof(Vector4) == 0
				+ sizeof(v)
			)
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}

	///////////////////////////////////

	Vector4::~Vector4 (void) {
	}

	///////////////////////////////////

	Vector4::Vector4 (Vector4 const& other):
#pragma warning( push )
#pragma warning( disable: 4351 ) // elements of "v" will be default initialised
		v() {
#pragma warning( pop )
		P_STATIC_ASSERT(sizeof(Vector4) == sizeof(v))
		v[0] = other.v[0];
		v[1] = other.v[1];
		v[2] = other.v[2];
		v[3] = other.v[3];
	}

	///////////////////////////////////

	Vector4 Vector4::New (float const x, float const y, float const z, float const w) {
		return Vector4(x, y, z, w);
	}

	///////////////////////////////////

	Vector4 Vector4::New (float const v[4]) {
		return Vector4(v[0], v[1], v[2], v[3]);
	}

	///////////////////////////////////

	Vector4 Vector4::New (Vector4 const& other) {
		return Vector4(other);
	}

	///////////////////////////////////


	Vector4::operator ankh::math::trig::vec4 (void) const {
		return ankh::math::trig::vec4(v[0], v[1], v[2], v[3]);
	}

	///////////////////////////////////


	bool Vector4::operator == (Vector4 const& other) const {
		return	v[0] == other.v[0]
			&&	v[1] == other.v[1]
			&&	v[2] == other.v[2]
			&&	v[3] == other.v[3]
			;
	}

	///////////////////////////////////


	Vector4 const Vector4::normalised (void) const {
		float const mag = magnitude();
		return New(x()/mag, y()/mag, z()/mag, w());
	}

	///////////////////////////////////

	// element-by-element multiplication

	Vector4 Vector4::mul (Vector4 const& other) const {
		return Vector4(v[0]*other.v[0], v[1]*other.v[1], v[2]*other.v[2], v[3]*other.v[3]);
	}

	///////////////////////////////////

	// scalar multiplication without touching the 4th element (w)

	Vector4 Vector4::mul (float const f) const {
		return Vector4(v[0]*f, v[1]*f, v[2]*f, v[3]);
	}

	///////////////////////////////////


	Vector4 Vector4::operator + (Vector4 const& other) const {
		return Vector4(v[0]+other.v[0], v[1]+other.v[1], v[2]+other.v[2], v[3]+other.v[3]);
	}

	///////////////////////////////////


	void Vector4::operator += (Vector4 const& other) {
		v[0] += other.v[0];
		v[1] += other.v[1];
		v[2] += other.v[2];
		v[3] += other.v[3];
	}

	///////////////////////////////////


	void Vector4::addtothis_asvec3 (Vector4 const& other) {
		v[0] += other.v[0];
		v[1] += other.v[1];
		v[2] += other.v[2];
	}

	///////////////////////////////////


	void Vector4::operator = (Vector4 const& other) {
		this->~Vector4();
		new(this) Vector4(other);
	}

	///////////////////////////////////


	float Vector4::magnitude (void) const {
		return sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) / abs(v[3]);
	}

	///////////////////////////////////


	// Inner (dot) product
	float Vector4::operator * (Vector4 const& other) const {
		return v[0]*other.v[0] + v[1]*other.v[1] + v[2]*other.v[2] + v[3]*other.v[3];
	}

	///////////////////////////////////


	Vector4 Vector4::operator * (float const f) const {
		return Vector4(v[0]*f, v[1]*f, v[2]*f, v[3]*f);
	}

	///////////////////////////////////


	float Vector4::angle (Vector4 const& other) const {
		return acos( operator *(other) / (magnitude() * other.magnitude()) );
	}

	///////////////////////////////////


	float Vector4::r (void) const {
		return v[0];
	}

	///////////////////////////////////


	float Vector4::g (void) const {
		return v[1];
	}

	///////////////////////////////////


	float Vector4::b (void) const {
		return v[2];
	}

	///////////////////////////////////


	float Vector4::a (void) const {
		return v[3];
	}

	///////////////////////////////////


	float Vector4::x (void) const {
		return v[0];
	}

	///////////////////////////////////


	float Vector4::y (void) const {
		return v[1];
	}

	///////////////////////////////////


	float Vector4::z (void) const {
		return v[2];
	}

	///////////////////////////////////


	float Vector4::w (void) const {
		return v[3];
	}

	///////////////////////////////////


	float Vector4::s (void) const {
		return v[0];
	}

	///////////////////////////////////


	float Vector4::t (void) const {
		return v[1];
	}

	///////////////////////////////////


	float Vector4::p (void) const {
		return v[2];
	}

	///////////////////////////////////


	float Vector4::q (void) const {
		return v[3];
	}

	///////////////////////////////////


	Vector4::Setter Vector4::x (void) {
		return Setter(v[0]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::y (void) {
		return Setter(v[1]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::z (void) {
		return Setter(v[2]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::w (void) {
		return Setter(v[3]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::r (void) {
		return Setter(v[0]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::g (void) {
		return Setter(v[1]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::b (void) {
		return Setter(v[2]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::a (void) {
		return Setter(v[3]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::s (void) {
		return Setter(v[0]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::t (void) {
		return Setter(v[1]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::p (void) {
		return Setter(v[2]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::q (void) {
		return Setter(v[3]);
	}

	///////////////////////////////////


	Vector4::Setter Vector4::operator [] (size_t i) {
		bool null(i < sizeof(v)/sizeof(v[0]));
		return Setter(v[i], null);
	}

	///////////////////////////////////


	float Vector4::operator [] (size_t i) const {
		return i < sizeof(v)/sizeof(v[0])? v[i] : static_cast<float>(0xffffffff);
	}

	///////////////////////////////////


	Vector4::Setter::Setter (float& v, bool _null):
		e(v),
		null(_null)
		{ P_STATIC_ASSERT(sizeof(Setter) == 3 + sizeof(float&) + sizeof(null)) }

	///////////////////////////////////


	float Vector4::Setter::operator = (float const v) const {
		return null? static_cast<float>(0xffffffff) : e = v;
	}

	///////////////////////////////////


	Vector4 Vector4::operator - (Vector4 const& other) const {
		return Vector4(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2], v[3] - other.v[3]);
	}

	///////////////////////////////////


	Vector4 const Vector4::sub_asvec3 (Vector4 const& other) const {
		return Vector4(v[0]/v[3] - other.v[0]/other.v[3], v[1]/v[3] - other.v[1]/other.v[3], v[2]/v[3] - other.v[2]/other.v[3], 1.0f);
	}
	///////////////////////////////////

}}} // namespace my::gl::math

