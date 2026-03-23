// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "Keep",
    platforms: [.iOS(.v17), .macOS(.v14)],
    products: [
        .library(name: "Keep", targets: ["Keep"]),
    ],
    dependencies: [],
    targets: [
        .target(
            name: "Keep",
            path: "Sources",
            resources: [.process("../Resources")]
        ),
        .testTarget(
            name: "KeepTests",
            dependencies: ["Keep"],
            path: "Tests"
        ),
    ]
)
