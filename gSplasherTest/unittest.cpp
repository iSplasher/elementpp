#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utils\Primitives.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
USING_NAMESPACE

namespace gSplasherTest
{		
	TEST_CLASS(Primitives)
	{
	public:
		
		TEST_METHOD(Adding)
		{
			auto tPoint = [](gPoint p, gPoint p2, int x, int y) {
				auto cp = p + p2;
				Assert::AreEqual(cp.x, x);
				Assert::AreEqual(cp.y, y);
			};

			tPoint(gPoint(20, 50), gPoint(50, 50), 70, 100);

			auto tSize = [](gSize p, gSize p2, unsigned w, unsigned h) {
				auto cp = p + p2;
				Assert::AreEqual(cp.width, w);
				Assert::AreEqual(cp.height, h);
			};

			tSize(gSize(200, 500), gSize(500,500), 700, 1000);

			gRect ar(20, 50, 200, 200);
			gRect br(50, 50, 200, 150);
			auto cr = ar + br;
			tPoint(cr.pos(), cr.pos(), 140, 200);
			tSize(gSize(cr.size()), gSize(cr.size()), 400, 350);

		}

	};
}